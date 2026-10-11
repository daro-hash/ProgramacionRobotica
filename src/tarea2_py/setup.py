from setuptools import find_packages, setup

package_name = 'tarea2_py'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='daro',
    maintainer_email='dario.bugarin9072@alumnos.udg.mx',
    description='TODO: Package description',
    license='Apache-2.0',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
    'console_scripts': [
        'service = tarea2_py.service_member_function:main',
        'client = tarea2_py.client_member_function:main',
        ],
    },
)
