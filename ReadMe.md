# Unity3D Webplayer Loader Patch

## 这是什么？

自从Unity3D放弃对Webplayer插件的支持后，Webplayer插件的更新服务器经常出现503错误，但为了使Webplayer能够继续使用，我们需要搭建自己的Webplayer更新服务器。

## 不是已经放弃支持了么，为什么还要搭建更新服务器？

Unity3D Webplayer Installer 中包含的并非程序的完整打包，而只是一个下载器（Loader），需要在线下载两个组件（Player & Mono Editor）后，Webplayer插件才能正常使用。

解包并重新打包安装器个人觉得不太明智，特别是对于部分商业公司来说。

## 如何使用？

### step 1. 搭建自己的 Unity3D Webplayer 更新服务器：

1. 下载更新服务器镜像数据
> [百度网盘](https://pan.baidu.com/s/1w0mlPjtG_7G75tHgAxQXog)    
> 提取码：7ve2
2. 将数据放在你的更新服务器的http、https服务的公开目录下。例如，若放在 www.example.com 网的 /root 子目录下，则应保证 www.example.com/root/revisions.plist 是可以访问的，同时不建议子目录过长。

注意：**域名与子目录共计长度不能超过 20 个字符，否则程序会崩溃，所以强烈建议就放在根目录下！**。

### step 2. 本地安装 Unity3D Webplayer 客户端：

UnityWebplayerFull.exe

> [百度网盘](https://pan.baidu.com/s/1KOsm3fs4dxrjknyf-h2ncQ)    
> 提取码：a9pv

正常安装，无需其他特殊操作。

### step 3. 给本地 Unity3D Webplayer 打补丁：

PatchWebplayerDll.exe [Release](https://github.com/Trickness/Unity3D-Webplayer-Loader-Patch/releases)

```
> PatchWebplayerDll.exe --replaced-with=www.example.com/root
```

--replaced-with 应包含域名（若使用https服务则不能为IP地址），且可包含子目录，后无斜杠，同时应保证该字符串后加上 "/revisions.plist" 是可以正常访问的。

**注意，请将 www.example.com/root 替换为您的更新服务器地址**

之后就可以正常运行了 XD

## 其他问题：

这里打包的更新服务器的镜像数据仅为版本编号为“ad2d0368e248”的数据，并没有将更新服务器上的所有数据都打包下来，如果出现无法下载文件404的错误，请根据错误提示，去官网上下载相对应版本号的数据，再放到你服务器上。
