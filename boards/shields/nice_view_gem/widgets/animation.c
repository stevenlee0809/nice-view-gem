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

static const lv_img_dsc_t *anim_imgs_forward[] = {
    &crystal_01, &crystal_02, &crystal_03, &crystal_04, &crystal_05, &crystal_06,
    &crystal_07, &crystal_08, &crystal_09, &crystal_10, &crystal_11, &crystal_12,
    &crystal_13, &crystal_14, &crystal_15, &crystal_16,
} ;

static const lv_img_dsc_t *anim_imgs_reverse[] = {
    &crystal_16, &crystal_15, &crystal_14, &crystal_13, &crystal_12, &crystal_11,
    &crystal_10, &crystal_09, &crystal_08, &crystal_07, &crystal_06, &crystal_05,
    &crystal_04, &crystal_03, &crystal_02, &crystal_01,
};

#if IS_ENABLED(CONFIG_NICE_VIEW_GEM_ANIMATION)
static lv_obj_t *anim_obj;
static bool reverse_playback;

static void restart_animation(void) {
    if (anim_obj == NULL) {
        return;
    }

    const lv_img_dsc_t **src = reverse_playback ? anim_imgs_reverse : anim_imgs_forward;
    lv_animimg_set_src(anim_obj, (const void **)src, 16);
    lv_animimg_set_duration(anim_obj, CONFIG_NICE_VIEW_GEM_ANIMATION_MS);
    lv_animimg_set_repeat_count(anim_obj, LV_ANIM_REPEAT_INFINITE);
    lv_animimg_start(anim_obj);
}
#endif

void set_animation_reverse(bool reverse) {
#if IS_ENABLED(CONFIG_NICE_VIEW_GEM_ANIMATION)
    if (reverse_playback == reverse) {
        return;
    }

    reverse_playback = reverse;
    restart_animation();
#else
    ARG_UNUSED(reverse);
#endif
}

void draw_animation(lv_obj_t *canvas) {
#if IS_ENABLED(CONFIG_NICE_VIEW_GEM_ANIMATION)
    lv_obj_t *art = lv_animimg_create(canvas);
    lv_obj_center(art);
    anim_obj = art;
    reverse_playback = false;
    restart_animation();
#else
    lv_obj_t *art = lv_img_create(canvas);

    int length = sizeof(anim_imgs_forward) / sizeof(anim_imgs_forward[0]);
    srand(k_uptime_get_32());
    int random_index = rand() % length;
    int configured_index = (CONFIG_NICE_VIEW_GEM_ANIMATION_FRAME - 1) % length;
    int anim_imgs_index = CONFIG_NICE_VIEW_GEM_ANIMATION_FRAME > 0 ? configured_index : random_index;

    lv_img_set_src(art, anim_imgs_forward[anim_imgs_index]);
#endif

    lv_obj_align(art, LV_ALIGN_TOP_LEFT, 36, 0);
}
