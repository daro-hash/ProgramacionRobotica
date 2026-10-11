#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "actividad3_interfaces/msg/hardware_status.hpp"  // Incluimos la libreria del mensaje Hardware Status

using namespace std::chrono_literals;

class HardwarePublisher : public rclcpp::Node //Creamos la clase del publisher
{
  public:
    HardwarePublisher()
    : Node("Nodo_Emulador_Hardware"), count_(0)
    {
      publisher_ = this->create_publisher<actividad3_interfaces::msg::HardwareStatus>("hardware_status", 10);
      timer_ = this->create_wall_timer(
      500ms, std::bind(&HardwarePublisher::timer_callback, this));  // Enviará una actualización cada 2 Hz
    }

  private:
    void timer_callback()
    {
      auto message = actividad3_interfaces::msg::HardwareStatus();
      this-> count_+= 5;
      message.temperatura = (count_) % 100;                         // La temperatura aumentará 10 grados cada medio segundo, cuando llegue a 120 regresará a 0
      if (count_ % 2 == 0) {
        message.motores_listos = 0; // Si el número es par, los motores se bloquean
      } else {
        message.motores_listos = 1;
      }
      if (message.temperatura < 25) {                                     // Condiciones para el debug_msg a partir de la temperatura
        message.debug_msg = "Calentando...";
      } else if (message.temperatura >= 25 && message.temperatura < 87) {
        message.debug_msg = "Operacion normal"; 
      } else {
        message.debug_msg = "Fallo de sensor";
      }
      RCLCPP_INFO_STREAM(this->get_logger(),                             // Publicamos el mensaje en la terminal
        "Publishing:\nTemperatura: " << message.temperatura << 
        "\nMotores: " << message.motores_listos << 
        "\nDebug: " << message.debug_msg);
        
      publisher_->publish(message);                                     // Publicamos el mensaje en el topico
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<actividad3_interfaces::msg::HardwareStatus>::SharedPtr publisher_;
    size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<HardwarePublisher>());
  rclcpp::shutdown();
  return 0;
}