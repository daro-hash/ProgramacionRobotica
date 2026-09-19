// Codigo adaptado del ejemplo obtenido en https://docs.ros.org/en/foxy/Tutorials/Beginner-Client-Libraries/Writing-A-Simple-Cpp-Publisher-And-Subscriber.html
//////// LIBRERIAS Y DEPENDENCIAS ////////
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
using std::placeholders::_1;

class CppSubscriber : public rclcpp::Node     // El codigo es similar al del publisher, con algunas adaptaciones, como los nombres de las variables y la clase <y el tipo de 
{                                             // servicio ejecutado en la clase, en este caso se utiliza create_subscription, sin embargo, el tipo de mensaje (String),
  public:                                     // nombre del topic y demas se queda igual. Ademas, en esta clase no hay un timer porque el subscriber siempre esta escuchando.
    CppSubscriber()
    : Node("Peter_Parker")
    {
      subscription_ = this->create_subscription<std_msgs::msg::String>(
      "NYC_News", 10, std::bind(&CppSubscriber::topic_callback, this, _1));
    }

  private:
    void topic_callback(const std_msgs::msg::String::SharedPtr msg) const
    {
      RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg->data.c_str());
    }
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char * argv[])             // El main es el mismo que en el publisher
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CppSubscriber>());
  rclcpp::shutdown();
  return 0;
}