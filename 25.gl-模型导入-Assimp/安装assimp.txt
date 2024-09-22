
1.下载源码
git clone https://github.com/assimp/assimp.git

2.生成Xcode过程

cd assimp                                     
cmake -G Xcode .

3.编译动态库 
选择assimp 编译

4.在Xcode的products中获取产物,修改名字为libassimpd.5.dylib
libassimpd.5.4.3.dylib
因为在动态加载时 libassimpd.5.4.3.dylib,会被动态的加载错误为libassimpd.5

5.添加include
在assimp工作中的include拖到目标工程

6.修改header search path
assimp/  加载其他文件里的路径
assimp/include/assimp 加载开发时引入的路径
