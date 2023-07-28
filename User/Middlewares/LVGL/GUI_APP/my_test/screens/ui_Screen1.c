// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.1
// LVGL version: 8.3.6
// Project name: my_test

#include "../ui.h"

void ui_Screen1_screen_init(void)
{
    ui_Screen1 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC |
                      LV_OBJ_FLAG_SCROLL_MOMENTUM);      /// Flags
    lv_obj_add_event_cb(ui_Screen1, scr_unloaded_delete_cb, LV_EVENT_SCREEN_UNLOADED, &ui_Screen1);

    ui_select = lv_tabview_create(ui_Screen1, LV_DIR_LEFT, 50);
    lv_obj_set_width(ui_select, 320);
    lv_obj_set_height(ui_select, 240);
    lv_obj_set_align(ui_select, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_select, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_text_color(ui_select, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_select, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_text_color(lv_tabview_get_tab_btns(ui_select), lv_color_hex(0xFF0000),
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(lv_tabview_get_tab_btns(ui_select), 255,  LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DAC = lv_tabview_add_tab(ui_select, "DAC");
    lv_obj_clear_flag(ui_DAC, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                      LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC |
                      LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags

    ui_Wave_tab = lv_label_create(ui_DAC);
    lv_obj_set_width(ui_Wave_tab, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Wave_tab, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Wave_tab, -91);
    lv_obj_set_y(ui_Wave_tab, -84);
    lv_obj_set_align(ui_Wave_tab, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Wave_tab, "Wave");
    lv_obj_set_style_text_color(ui_Wave_tab, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Wave_tab, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Wave_select = lv_dropdown_create(ui_DAC);
    lv_dropdown_set_options(ui_Wave_select, "sine\nsweep");
    lv_obj_set_width(ui_Wave_select, 117);
    lv_obj_set_height(ui_Wave_select, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Wave_select, 57);
    lv_obj_set_y(ui_Wave_select, -82);
    lv_obj_set_align(ui_Wave_select, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Wave_select, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_set_style_text_color(ui_Wave_select, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Wave_select, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_OUT_switch = lv_switch_create(ui_DAC);
    lv_obj_set_width(ui_OUT_switch, 50);
    lv_obj_set_height(ui_OUT_switch, 25);
    lv_obj_set_x(ui_OUT_switch, 80);
    lv_obj_set_y(ui_OUT_switch, 90);
    lv_obj_set_align(ui_OUT_switch, LV_ALIGN_CENTER);

    lv_obj_set_style_bg_color(ui_OUT_switch, lv_color_hex(0xFF0000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_OUT_switch, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    ui_OUT_tab = lv_label_create(ui_DAC);
    lv_obj_set_width(ui_OUT_tab, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_OUT_tab, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_OUT_tab, -85);
    lv_obj_set_y(ui_OUT_tab, 89);
    lv_obj_set_align(ui_OUT_tab, LV_ALIGN_CENTER);
    lv_label_set_text(ui_OUT_tab, "OUT:OFF");
    lv_obj_set_style_text_color(ui_OUT_tab, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_OUT_tab, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Fre_tab = lv_label_create(ui_DAC);
    lv_obj_set_width(ui_Fre_tab, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Fre_tab, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Fre_tab, -88);
    lv_obj_set_y(ui_Fre_tab, -32);
    lv_obj_set_align(ui_Fre_tab, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Fre_tab, "Fre(MHz)");
    lv_obj_set_style_text_color(ui_Fre_tab, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Fre_tab, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Fre = lv_spinbox_create(ui_DAC);
    lv_obj_set_width(ui_Fre, 69);
    lv_obj_set_height(ui_Fre, 38);
    lv_obj_set_x(ui_Fre, 48);
    lv_obj_set_y(ui_Fre, -31);
    lv_obj_set_align(ui_Fre, LV_ALIGN_CENTER);
    lv_spinbox_set_digit_format(ui_Fre, 5, 2);
    lv_spinbox_set_range(ui_Fre, 0, 99999);
    lv_spinbox_set_step(ui_Fre, round(pow(10, 1 - 1)));

    ui_Fre_inc_btn = lv_btn_create(ui_DAC);
    lv_obj_set_width(ui_Fre_inc_btn, 30);
    lv_obj_set_height(ui_Fre_inc_btn, 20);
    lv_obj_set_x(ui_Fre_inc_btn, 106);
    lv_obj_set_y(ui_Fre_inc_btn, -31);
    lv_obj_set_align(ui_Fre_inc_btn, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Fre_inc_btn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Fre_inc_btn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Fre_inc_tab = lv_label_create(ui_Fre_inc_btn);
    lv_obj_set_width(ui_Fre_inc_tab, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Fre_inc_tab, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Fre_inc_tab, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Fre_inc_tab, "+");

    ui_Fre_dec_btn = lv_btn_create(ui_DAC);
    lv_obj_set_width(ui_Fre_dec_btn, 30);
    lv_obj_set_height(ui_Fre_dec_btn, 20);
    lv_obj_set_x(ui_Fre_dec_btn, -10);
    lv_obj_set_y(ui_Fre_dec_btn, -31);
    lv_obj_set_align(ui_Fre_dec_btn, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Fre_dec_btn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Fre_dec_btn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Fre_dec_tab = lv_label_create(ui_Fre_dec_btn);
    lv_obj_set_width(ui_Fre_dec_tab, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Fre_dec_tab, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Fre_dec_tab, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Fre_dec_tab, "-");

    ui_Vol_tab = lv_label_create(ui_DAC);
    lv_obj_set_width(ui_Vol_tab, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Vol_tab, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Vol_tab, -90);
    lv_obj_set_y(ui_Vol_tab, 19);
    lv_obj_set_align(ui_Vol_tab, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Vol_tab, "Vol");
    lv_obj_set_style_text_color(ui_Vol_tab, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Vol_tab, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Slider2 = lv_slider_create(ui_DAC);
    lv_obj_set_width(ui_Slider2, 150);
    lv_obj_set_height(ui_Slider2, 10);
    lv_obj_set_x(ui_Slider2, 47);
    lv_obj_set_y(ui_Slider2, 19);
    lv_obj_set_align(ui_Slider2, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_Slider2, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Slider2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ADC = lv_tabview_add_tab(ui_select, "ADC");
    lv_obj_clear_flag(ui_ADC, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                      LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC |
                      LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags

    ui_Label2 = lv_label_create(ui_ADC);
    lv_obj_set_width(ui_Label2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label2, -81);
    lv_obj_set_y(ui_Label2, -91);
    lv_obj_set_align(ui_Label2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label2, "Measure");
    lv_obj_set_style_text_color(ui_Label2, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_measure = lv_switch_create(ui_ADC);
    lv_obj_set_width(ui_measure, 50);
    lv_obj_set_height(ui_measure, 25);
    lv_obj_set_x(ui_measure, 85);
    lv_obj_set_y(ui_measure, -90);
    lv_obj_set_align(ui_measure, LV_ALIGN_CENTER);

    lv_obj_set_style_bg_color(ui_measure, lv_color_hex(0xFF0000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_measure, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_Wave_select, ui_event_Wave_select, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_OUT_switch, ui_event_OUT_switch, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Fre, ui_event_Fre, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Fre_inc_btn, ui_event_Fre_inc_btn, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Fre_dec_btn, ui_event_Fre_dec_btn, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Slider2, ui_event_Slider2, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_measure, ui_event_measure, LV_EVENT_ALL, NULL);

}
