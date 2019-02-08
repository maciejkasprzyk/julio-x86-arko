/* Simple Allegro 5 tilemap example from allegro.cc:
 *
 * http://www.allegro.cc/forums/thread/606482
 *
 * Also see here for more info:
 *
 * http://wiki.allegro.cc/index.php?title=Allegro_5_Tutorial
 *
 * Place fixed_font.tga and icon.tga from the Allegro 5 examples/data
 * folder next to the .exe and there will be an FPS counter and an
 * icon.
 *
 * Left mouse = Pan
 * Right mouse = Rotozoom
 * Esc = Quit
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "allegro5/allegro.h"
#include "x86_function.h"

/* Our window. */
ALLEGRO_DISPLAY *display;

double scale = 4.0/800.0; //0.005
double center_x = 0, center_y = 0;

double cReal, cImaginary;
//ciekawe wartosci
//c = -0.074000 + -0.660000i << to super
//c = -0.076000 + 0.659000i
//c = 0.080000 + 0.623000i
//c = -0.067000 + -0.668000i

//double cReal= -0.3;
//double cImaginary = 0.65;

//double cReal= -0.15;
//double cImaginary = 0.65;

//double cReal= -0.13;
//double cImaginary = 0.65;

//double cReal= -0.123;
//double cImaginary = 0.745;


void draw() {
    al_reset_clipping_rectangle();
    al_clear_to_color(al_map_rgb(0,0,0));

    ALLEGRO_LOCKED_REGION *lockedRegion = al_lock_bitmap(al_get_backbuffer(display), ALLEGRO_PIXEL_FORMAT_RGB_888, ALLEGRO_LOCK_READWRITE);

    if(lockedRegion==NULL)
    {
        printf("Error, cant lock bitmap\n");
        exit(1);
    }

    int displayWidth = al_get_display_width(display);
    int displayHeight = al_get_display_height(display);
    double leftX = center_x - displayWidth * scale/2.0;
    double upY = center_y + displayHeight * scale/2.0;
//    printf("center_x %d\n", center_x);
//    printf("center_y %d\n", center_y);
//    printf("pitch %d\n", lockedRegion->pitch);
//    printf("scale %.17g\n", scale);
    printf("c = %f + %fi\n", cReal, cImaginary);
    char * p = lockedRegion->data;

    x86_function(leftX, upY, displayWidth , displayHeight, scale, p, lockedRegion->pitch, cReal, cImaginary );

    al_unlock_bitmap(al_get_backbuffer(display));
}

int main(int argc, char * argv[]) {
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *queue;
    bool redraw = true;

    srand(time(NULL));

    /* Init Allegro 5 + addons. */
    al_init();
    al_install_mouse();
    al_install_keyboard();

    /* Create our window. */
    //al_set_new_display_flags(ALLEGRO_RESIZABLE);
    display = al_create_display(600, 600);
    al_set_window_title(display, "Zbiory Julii");



    //al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    timer = al_create_timer(1.0 / 30);
    queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    /* FPS counter */
    int fpsAccum = 0;
    double fpsTime = 0.0;

    int mouseButton = 0;
    while (1) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                break;
        }
        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            mouseButton = event.mouse.button;
        }
        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            mouseButton = 0;
        }
        if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
            /* Left button scrolls. */
            if (mouseButton == 1) {
                center_x -= event.mouse.dx * scale;
                center_y += event.mouse.dy * scale;
                redraw = true;
            }
            /* Right button changes c */
            if (mouseButton == 2) {
                cReal += event.mouse.dx * scale / 5.0 ;//0.001;
                cImaginary -= event.mouse.dy * scale / 5.0 * 2.0;//0.002;
                redraw = true;
            }
            if(event.mouse.dz!= 0){
                scale -= event.mouse.dz * 0.1 * scale;
                redraw = true;
            }
            if (scale < 5.154941427863653e-14) scale =5.154941427863653e-14;

        }
        if (event.type == ALLEGRO_EVENT_TIMER){
            //redraw = true;
        }

        if (redraw && al_is_event_queue_empty(queue)) {
            redraw = false;
            draw();
            al_flip_display();



            double t = al_get_time();
            fpsAccum++;
            if (t - fpsTime >= 1) {
                printf("%d\n", fpsAccum);
                //printf("c = %f + %fi\n", cReal, cImaginary);
                fpsAccum = 0;
                fpsTime = t;
            }
        }
    }
    return 0;
}