#define NUM_PIXELS 38
#define DEFAULT_BRIGHTNESS 1.0

#include "../RaspberryPi-NeoPixel-WS2812/ws2812-RPi.c"
#include "../RaspberryPi-NeoPixel-WS2812/ws2812-RPi-effects.c"

int main(int argc, char **argv) {
	// Check "Single Instance"
	int pid_file = open("/var/run/ws2812RPi.pid", O_CREAT | O_RDWR, 0666);
	int rc = flock(pid_file, LOCK_EX | LOCK_NB);
	if(rc) {
	    if(EWOULDBLOCK == errno)
	    {
	        // another instance is running
	        printf("Instance already running\n");
	        exit(EXIT_FAILURE);
	    }
	}

	// Catch all signals possible - it's vital we kill the DMA engine on process exit!
	int i;
	for (i = 0; i < 64; i++) {
		struct sigaction sa;
		memset(&sa, 0, sizeof(sa));
		sa.sa_handler = terminate;
		sigaction(i, &sa, NULL);
	}

	// Don't buffer console output
	setvbuf(stdout, NULL, _IONBF, 0);

	// How many LEDs?
	numLEDs = NUM_PIXELS;

	// How bright? (Recommend 0.2 for direct viewing @ 3.3V)
	setBrightness(DEFAULT_BRIGHTNESS);

	// Init PWM generator and clear LED buffer
	initHardware();
	clearLEDBuffer();

	while(true) {
		rainbowCycle(5);
	}

	// Exit cleanly, freeing memory and stopping the DMA & PWM engines
	// We trap all signals (including Ctrl+C), so even if you don't get here, it terminates correctly
	terminate(0);
	return 0;
}