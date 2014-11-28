#define NUM_PIXELS 38
#define DEFAULT_BRIGHTNESS 1.0

#define loops 1

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

	// time_t t = time(NULL);
	// struct tm tm = *localtime(&t);
	// printf("Start: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	while(true) {
		rainbowCycle_wipe(5);
		// tm = *localtime(&t);
		// printf("rainbowCycle_wipe() Done: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

		for (i = 0; i < loops; i++) {
			rainbowCycle_f(5);
		}
		// tm = *localtime(&t);
		// printf("rainbowCycle_f() Done: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

		// for (i = 0; i < loops; i++) {
		// 	rainbowCycle(5);
		// }
		// time_t t = time(NULL);
		// struct tm tm = *localtime(&t);
		// printf("rainbowCycle() Done: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

		for (i = 0; i < loops; i++) {
			colorWipe(Color(255, 0, 0), 50);
			colorWipe_r(Color(0, 255, 0), 50);
			colorWipe(Color(0, 0, 255), 50);
			colorWipe_r(Color(255, 255, 255), 50);
			colorWipe(Color(255, 255, 0), 50);
			colorWipe_r(Color(255, 0, 255), 50);
			colorWipe(Color(0, 255, 255), 50);
			colorWipe_r(Color(255, 125, 0), 50);
			colorWipe(Color(185, 60, 255), 50);
		}
		// tm = *localtime(&t);
		// printf("colorWipe(s) Done: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	}

	// Exit cleanly, freeing memory and stopping the DMA & PWM engines
	// We trap all signals (including Ctrl+C), so even if you don't get here, it terminates correctly
	terminate(0);
	return 0;
}