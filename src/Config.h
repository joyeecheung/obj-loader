#ifndef CONFIG_H
#define CONFIG_H

const int WIDTH = 800, HEIGHT = 600, MENU_HEIGHT = 25;
const float MAX_NORMAL_ANGLE = 179.0f;

/***********************
* Lighting configuration
***********************/
const float LIGHT_AMBIENT_1[] = { 0.1f, 0.1f, 0.1f, 0.0f };
const float LIGHT_DIFFUSE_1[] = {
    183.0f / 255.0f, 183.0f / 255.0f, 183.0f / 255.0f, 0.0f
};
const float LIGHT_POSITION_1[] = { 0.8f, 0.4f, 0.5f, 0.0f };

const float LIGHT_AMBIENT_2[] = { 0.2f, 0.2f, 0.2f, 0.0f };
const float LIGHT_DIFFUSE_2[] = {
    211.0f / 255.0f, 211.0f / 255.0f, 211.0f / 255.0f, 0.0f
};
const float LIGHT_POSITION_2[] = { 0.8f, -0.2f, 0.5f, 0.0f };

const float MOVE_SPEED = 0.1f;
const float MID_MOVE_SPEED = 0.005f;
const float ROTATE_SPEED = 1.0f;
const float ROTATE_FACTOR = 0.25f;
const float CAMERA_POS[] = { 0.0f, 0.0f, 5.0f };
const float BACKGROUND[] = { 0.2f, 0.2f, 0.2f, 0.0f };
const float FOREGROUND[] = { 1.0f, 1.0f, 1.0f, 1.0f };

const float INFO_POSITION_Z = 0.0f;
const float POS_INFO_POSITION[] = { 0.02f, 0.90f, INFO_POSITION_Z };
const float ROT_INFO_POSITION[] = { 0.02f, 0.95f, INFO_POSITION_Z };
const float SHADE_INFO_POSITION[] = { 0.75f, 0.03f, INFO_POSITION_Z };
const float LIGHT_INFO_POSITION[] = { 0.75f, 0.08f, INFO_POSITION_Z };

#endif