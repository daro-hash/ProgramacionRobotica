import rclpy 
from rclpy.node import Node

class TestNode(Node):
    def __init__(self):
        super().__init__("Nodo_python")
        self.get_logger().info('Nodo lol again')
def main():
    rclpy.init()
    node = TestNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()
if __name__ == '__main__':
    main()
