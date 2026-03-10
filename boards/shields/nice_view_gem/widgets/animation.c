#include <stdlib.h>
#include <zephyr/kernel.h>
#include "animation.h"

LV_IMG_DECLARE(crystal_01);
LV_IMG_DECLARE(crystal_02);
LV_IMG_DECLARE(crystal_03);
LV_IMG_DECLARE(crystal_04);
LV_IMG_DECLARE(crystal_05);
LV_IMG_DECLARE(crystal_06);
LV_IMG_DECLARE(crystal_07);
LV_IMG_DECLARE(crystal_08);
LV_IMG_DECLARE(crystal_09);
LV_IMG_DECLARE(crystal_10);
LV_IMG_DECLARE(crystal_11);
LV_IMG_DECLARE(crystal_12);
LV_IMG_DECLARE(crystal_13);
LV_IMG_DECLARE(crystal_14);
LV_IMG_DECLARE(crystal_15);
LV_IMG_DECLARE(crystal_16);

const lv_img_dsc_t *anim_imgs[] = {
    &crystal_01, &crystal_02, &crystal_03, &crystal_04, &crystal_05, &crystal_06,
    &crystal_07, &crystal_08, &crystal_09, &crystal_10, &crystal_11, &crystal_12,
    &crystal_13, &crystal_14, &crystal_15, &crystal_16,
};

const lv_img_dsc_t *anim_imgs_reverse[] = {
    &crystal_16, &crystal_15, &crystal_14, &crystal_13, &crystal_12, &crystal_11,
    &crystal_10, &crystal_09, &crystal_08, &crystal_07, &crystal_06, &crystal_05,
    &crystal_04, &crystal_03, &crystal_02, &crystal_01,
};

static lv_obj_t *anim_obj;
static bool reverse_playing;
static bool reverse_replay_pending;

static void animation_ready_cb(lv_event_t *event) {
#if IS_ENABLED(CONFIG_NICE_VIEW_GEM_ANIMATION)
    ARG_UNUSED(event) ;
    if (!reverse_playing || anim_obj == NULL) {
        return;
    }

    if (reverse_replay_pending) {
        reverse_replay_pending = false;
        lv_animimg_stop(anim_obj);
        lv_animimg_set_src(anim_obj, (const void **)anim_imgs_reverse, 16);
        lv_animimg_set_repeat_count(anim_obj, 0);
        lv_animimg_start(anim_obj);
        return;
    }

    reverse_playing = false;
    lv_animimg_stop(anim_obj);
    lv_animimg_set_src(anim_obj, (const void **)anim_imgs, 16);
    lv_animimg_set_repeat_count(anim_obj, LV_ANIM_REPEAT_INFINITE);
    lv_animimg_start(anim_obj);
#else
    ARG_UNUSED(event);
#endif
}

void trigger_animation_reverse_once(void) {
#if IS_ENABLED(CONFIG_NICE_VIEW_GEM_ANIMATION)
    if (anim_obj == NULL) {
        return;
    }

    if (reverse_playing) {
        reverse_replay_pending = true;
        return;
    }

    reverse_playing = true;
    reverse_replay_pending = false;
    lv_animimg_stop(anim_obj);
    lv_animimg_set_src(anim_obj, (const void **)anim_imgs_reverse, 16);
    lv_animimg_set_repeat_count(anim_obj, 0);
    lv_animimg_start(anim_obj);
#endif
}

void draw_animation(lv_obj_t *canvas) {
#if IS_ENABLED(CONFIG_NICE_VIEW_GEM_ANIMATION)
    lv_obj_t *art = lv_animimg_create(canvas);
    lv_obj_center(art);

    anim_obj = art;
    reverse_playing = false;
    reverse_replay_pending = false;
    lv_animimg_set_src(art, (const void **)anim_imgs, 16);
    lv_animimg_set_duration(art, CONFIG_NICE_VIEW_GEM_ANIMATION_MS);
    lv_animimg_set_repeat_count(art, LV_ANIM_REPEAT_INFINITE);
    lv_obj_add_event_cb(art, animation_ready_cb, LV_EVENT_READY, NULL);
    lv_animimg_start(art);
#else
    lv_obj_t *art = lv_img_create(canvas);

    int length = sizeof(anim_imgs) / sizeof(anim_imgs[0]);
    srand(k_uptime_get_32());
    int random_index = rand() % length;

    lv_img_set_src(art, anim_imgs[random_index]);
#endif

    lv_obj_align(art, LV_ALIGN_TOP_LEFT, 36, 0);
}
