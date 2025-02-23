#include "iot/thing.h"
#include "board.h"
#include "audio_codec.h"

#include <driver/gpio.h>
#include <esp_log.h>

#define TAG "Lamp"

namespace iot {

// 这里仅定义 Lamp 的属性和方法，不包含具体的实现
class Lamp : public Thing {
private:
    gpio_num_t gpio_num_r = GPIO_NUM_27;
    gpio_num_t gpio_num_g = GPIO_NUM_16;
    gpio_num_t gpio_num_y = GPIO_NUM_14;
    bool power_ = false;

    void InitializeGpio() {
        gpio_config_t config_r = {
            .pin_bit_mask = (1ULL << gpio_num_r),
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
        };
        gpio_config_t config_g = {
            .pin_bit_mask = (1ULL << gpio_num_g),
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
        };
        gpio_config_t config_y = {
            .pin_bit_mask = (1ULL << gpio_num_y),
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
        };
        ESP_ERROR_CHECK(gpio_config(&config_r));
        ESP_ERROR_CHECK(gpio_config(&config_g));
        ESP_ERROR_CHECK(gpio_config(&config_y));
        gpio_set_level(gpio_num_r, 0);
        gpio_set_level(gpio_num_g, 0);
        gpio_set_level(gpio_num_y, 0);
    }

public:
    Lamp() : Thing("Lamp", "一个测试用的灯"), power_(false) {
        InitializeGpio();

        // 定义设备的属性
        properties_.AddBooleanProperty("power", "灯是否打开", [this]() -> bool {
            return power_;
        });

        // 定义设备可以被远程执行的指令
        methods_.AddMethod("TurnOn_R", "打开紅灯", ParameterList(), [this](const ParameterList& parameters) {
            power_ = true;
            gpio_set_level(gpio_num_r, 1);
        });
        methods_.AddMethod("TurnOn_G", "打开綠灯", ParameterList(), [this](const ParameterList& parameters) {
            power_ = true;
            gpio_set_level(gpio_num_g, 1);
        });
        methods_.AddMethod("TurnOn_Y", "打开黃灯", ParameterList(), [this](const ParameterList& parameters) {
            power_ = true;
            gpio_set_level(gpio_num_y, 1);
        });

        methods_.AddMethod("TurnOff_R", "关闭紅灯", ParameterList(), [this](const ParameterList& parameters) {
            power_ = false;
            gpio_set_level(gpio_num_r, 0);
        });
        methods_.AddMethod("TurnOff_G", "关闭綠灯", ParameterList(), [this](const ParameterList& parameters) {
            power_ = false;
            gpio_set_level(gpio_num_g, 0);
        });
        methods_.AddMethod("TurnOff_Y", "关闭黃灯", ParameterList(), [this](const ParameterList& parameters) {
            power_ = false;
            gpio_set_level(gpio_num_y, 0);
        });

        methods_.AddMethod("TurnOff_all", "关闭全部的灯", ParameterList(), [this](const ParameterList& parameters) {
            power_ = false;
            gpio_set_level(gpio_num_r, 0);
            gpio_set_level(gpio_num_g, 0);
            gpio_set_level(gpio_num_y, 0);
        });
    }
};

} // namespace iot

DECLARE_THING(Lamp);
