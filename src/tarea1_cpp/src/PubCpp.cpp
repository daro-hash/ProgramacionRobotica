// Codigo adaptado del ejemplo obtenido en https://docs.ros.org/en/foxy/Tutorials/Beginner-Client-Libraries/Writing-A-Simple-Cpp-Publisher-And-Subscriber.html
//////// LIBRERIAS Y DEPENDENCIAS ////////
#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp" // Esta dependencia incluye el tipo de mensaje "string", facilita la interpretacion

using namespace std::chrono_literals;


class CppPublisher : public rclcpp::Node  // Creamos una clase llamada CppPublisher que hereda las caracteristicas de Node. Cuando se utiliza "this" se esta refiriendo al CppPublisher
{
  public:
    CppPublisher()                        // Constructor de la clase
    : Node("The_Paper"), count_(0)        // Nombre del nodo
    {
      publisher_ = this->create_publisher<std_msgs::msg::String>("NYC_News", 10);   // Nombre del topic y creacion del publisher en la clase
      timer_ = this->create_wall_timer(
      1500ms, std::bind(&CppPublisher::timer_callback, this));                      // Usamos bind() para enlazar el publisher con la funcion timer_callback que es donde se genera el loop de mensajes
    }

  private:
    void timer_callback()
    {
      auto message = std_msgs::msg::String();                                       // Definimos el tipo de mensajes, en este caso strings
      message.data = "News from The Paper: " + std::to_string(count_++);            // Definimos la estructura del mensaje
      RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());    // Esta funcion copia el mensaje y lo publica en la terminal del publisher, facilita el debugging
      publisher_->publish(message);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    size_t count_;
};

int main(int argc, char * argv[])       // En la funcion main se da spin al nodo, inicializandolo una vez; como la funcion callback es un loop, entonces se mantiene inicializada
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CppPublisher>());
  rclcpp::shutdown();
  return 0;
}