#ifdef __cplusplus
extern "C" {
#endif

void display_init();
void spi_init();
void display_connect();
void display_splash_screen();

void display_clear();
void display_refresh();
void display_update();

void display_wake();
void display_sleep();

#ifdef __cplusplus
}
#endif
