ǰ����������Ҫ���а�װprotobuf,zookeeper

sudo apt-get install libzookeeper-mt-dev zookeeper zookeeperd
sudo apt-get install protobuf-compiler libprotobuf-dev

zookeeper����Ҫ�������أ�����Ŀ�õ���zookeeper3.8.4

___________________________________________________________________________________________________________
��ʹ����moduo���yaml-cpp�⣬��������������Ŀ�����ˣ�������������

______________________________________________________________________________________________________________
ʹ�����̣�
1.������zookeeper��ȷ����zookeeper��������ip�Ͷ˿�
2.д��yaml�����ļ���������rpc�ṩ�ߵ�ip�˿ں�zookeeper��ip�˿ڣ�д��������ʽ��

ip: 192.168.50.203
port: 8080
zkip: 192.168.50.203
zkport: 2181

3.�����Լ������������дprotobuf�����ɶ�Ӧ�ļ�
4.������дprovider��client�����ˡ�

______________________________________________________________________________________________________________
���ڽӿ�ʹ�ã����Բ鿴app/rpcĿ¼�µ�����