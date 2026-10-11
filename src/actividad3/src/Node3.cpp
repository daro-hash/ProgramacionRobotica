#include "rclcpp/rclcpp.hpp"
#include "actividad3_interfaces/srv/check_system.hpp"       //Incluimos la libreria del servicio personalizado
#include <chrono>
#include <memory>

using namespace std::chrono_literals;

class NodoOperador : public rclcpp::Node
{
public:
  NodoOperador() : Node("Operator_Node")
  {
    client_ = this->create_client<actividad3_interfaces::srv::CheckSystem>("Check_System");
    timer_ = this->create_wall_timer(                                                        // Temporizador para llamar al servicio cada 2.5 segundos
      2.5s, std::bind(&NodoOperador::send_request, this));
  }

private:
  void send_request()
  {
    if (!client_->wait_for_service(1s)) {
      RCLCPP_WARN(this->get_logger(), "Servicio no disponible, esperando...");
      return;
    }

    auto request = std::make_shared<actividad3_interfaces::srv::CheckSystem::Request>();
    client_->async_send_request(request,                                                            // Llamada asíncrona con un callback para manejar la respuesta sin bloquear el nodo
      [this](rclcpp::Client<actividad3_interfaces::srv::CheckSystem>::SharedFuture future) {
        try {
          auto response = future.get();
          if (response->is_safe) {
            RCLCPP_INFO(this->get_logger(), "Estado seguro, motores desbloqueados...");
          } else {
            RCLCPP_WARN(this->get_logger(), "Estado inseguro, motores bloqueados...");
          }
        } catch (const std::exception &e) {
          RCLCPP_ERROR(this->get_logger(), "Fallo al llamar al servicio: %s", e.what());
        }
      });
  }

  rclcpp::Client<actividad3_interfaces::srv::CheckSystem>::SharedPtr client_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<NodoOperador>());
  rclcpp::shutdown();
  return 0;
}