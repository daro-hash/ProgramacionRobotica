# Codigo adaptado del ejemplo obtenido en https://docs.ros.org/en/foxy/Tutorials/Beginner-Client-Libraries/Writing-A-Simple-Py-Publisher-And-Subscriber.html
###### LIBRERIAS Y DEPENDENCIAS ######
import rclpy
from rclpy.node import Node

from std_msgs.msg import String


class PythonSubscriber(Node):                               # El codigo es similar al PubPy, las diferencias se encuentran en la especificacion del subscriptor y en el nombre 
                                                            # del nodo, asi como en que, al igual que en el SubCpp, no necesitamos un loop, porque el listener esta escuchando en
    def __init__(self):                                     # todo momento
        super().__init__('Ben_Reily')
        self.subscription = self.create_subscription(
            String,
            'NYC_News',
            self.listener_callback,
            10)
        self.subscription  # prevent unused variable warning

    def listener_callback(self, msg):
        self.get_logger().info('I read: "%s"' % msg.data)


def main(args=None):                                        # Al igual que en SubCpp, el main es igual al del publisher
    rclpy.init(args=args)

    subscriber = PythonSubscriber()

    rclpy.spin(subscriber)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    subscriber.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()