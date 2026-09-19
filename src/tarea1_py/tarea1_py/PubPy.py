# Codigo adaptado del ejemplo obtenido en https://docs.ros.org/en/foxy/Tutorials/Beginner-Client-Libraries/Writing-A-Simple-Py-Publisher-And-Subscriber.html
###### LIBRERIAS Y DEPENDENCIAS ######
import rclpy
from rclpy.node import Node

from std_msgs.msg import String


class PythonPublisher(Node):    # Creamos una clase llamada PythonPublisher que hereda las caracteristicas de un nodo

    def __init__(self):                                                 # Definimos dos funciones, init y timer_callback, la primera especifica los datos del publisher, 
        super().__init__('Daily_Bugle')                                 # la segunda genera el loop de mensajes, como en PubCpp
        self.publisher_ = self.create_publisher(String, 'NYC_News', 10) # Para que los nodos de python y c++ se comuniquen entre si, necesitamos que ambos apunten al mismo topic
        timer_period = 1  # seconds                                     # Definimos una frecuencia de publicacion de 1 segundo
        self.timer = self.create_timer(timer_period, self.timer_callback)
        self.i = 0

    def timer_callback(self):                                           # En timer_callback se generan los mensajes en loop cada x cantidad de tiempo (definida en timer_period)
        msg = String()                                                  # Definimos el tipo de mensaje, debe ser el mismo que en el ejemplo de Cpp
        msg.data = 'News from the Daily Bugle: %d' % self.i
        self.publisher_.publish(msg)
        self.get_logger().info('Publishing: "%s"' % msg.data)           # Imprimimos el mensaje en terminal
        self.i += 1                                                     # Actualizamos el contador


def main(args=None):                                                    # La funcion main funciona de manera analoga a la del PubCpp
    rclpy.init(args=args)

    publisher = PythonPublisher()

    rclpy.spin(publisher)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    publisher.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()