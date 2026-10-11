#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "actividad3_interfaces/msg/hardware_status.hpp"  
#include "actividad3_interfaces/srv/check_system.hpp"       // Agregamos las librerías del mensaje y el servicio personalizados

using std::placeholders::_1;
using std::placeholders::_2;

class NodoMonitor : public rclcpp::Node
{
  public:
    NodoMonitor() : Node("Nodo_Monitor"), ultima_temp_(0), ultimos_motores_(false)
    {
      this->declare_parameter<int>("umbral_temperatura", 50);                       
      umbral_temperatura_ = this->get_parameter("umbral_temperatura").as_int();                 // Declarar y leer el parámetro que enviará el launch

      subscription_ = this->create_subscription<actividad3_interfaces::msg::HardwareStatus>(
        "hardware_status", 10, std::bind(&NodoMonitor::topic_callback, this, _1));              // Inicializamos el Suscriptor y lo sucribimos al topico.

      service_ = this->create_service<actividad3_interfaces::srv::CheckSystem>(
        "Check_System", std::bind(&NodoMonitor::check_callback, this, _1, _2));                 // También inicializamos el servicio, lo dejamos dentro de la clase NodoMonitor para que sea un solo nodo.
        
      RCLCPP_INFO(this->get_logger(), "Nodo Monitor iniciado. Umbral de temperatura: %d", umbral_temperatura_); // Imprimimos en terminal
    }

  private:
    int umbral_temperatura_;
    int64_t ultima_temp_;
    bool ultimos_motores_;
    
    rclcpp::Subscription<actividad3_interfaces::msg::HardwareStatus>::SharedPtr subscription_;
    rclcpp::Service<actividad3_interfaces::srv::CheckSystem>::SharedPtr service_;

    void topic_callback(const actividad3_interfaces::msg::HardwareStatus & message) 
    {
      // Guardar el último estado conocido
      ultima_temp_ = message.temperatura;
      ultimos_motores_ = message.motores_listos;

      // Evaluar advertencia
      if (ultima_temp_ > umbral_temperatura_) {
        RCLCPP_WARN(this->get_logger(), "¡ADVERTENCIA! La temperatura actual (%ld) superó el umbral (%d)", ultima_temp_, umbral_temperatura_);
      } else {
        RCLCPP_INFO(this->get_logger(), "Temperatura actual recibida: %ld", ultima_temp_);
      }
    }

    void check_callback(const std::shared_ptr<actividad3_interfaces::srv::CheckSystem::Request> request,
                        std::shared_ptr<actividad3_interfaces::srv::CheckSystem::Response> response)
    {
      (void)request;                                                                                      // Para evitar pedir datos al cliente, usamos los datos que ya habiamos recabado, de esa forma
      if (ultima_temp_ < umbral_temperatura_ && ultimos_motores_ == true) {                               // Optimizamos la memoria y evitamos redundancias
        response->is_safe = true;
      } else {
        response->is_safe = false;
      }
      
      RCLCPP_INFO_STREAM(this->get_logger(),                             // Publicamos el mensaje en la terminal
        "Peticion recibida... Enviando:\t" << response->is_safe);
    }
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<NodoMonitor>()); // Al hacer spin de esta clase, tanto el tópico como el servicio funcionan simultáneamente
  rclcpp::shutdown();
  return 0;
}