### QtQQ项目设计与实现
环境配置：Win10 + vs2019 + msvc + Qt Vs Tools
##### BasicWindow设计
·作为其他窗口的基类、内含鼠标移动事件及基本加载界面样式事件
##### CCMainWindow设计
·主窗口类设计，初始化主窗口界面。
##### CommonUtils设计
·获取默认的部件风格以及设置部件风格
##### ContactItem
·聊天窗口中界面上面的头像、名称等显示
##### CustomMenu
·添加系统托盘下右击点击显示的自定义功能。
##### EmotionLabelItem
·表情被点击执行操作。主要发送表情点击信号
##### EmotionWindow
·发送表情功能。设计表情界面的样式。
##### MsgWebView
·与其他人聊天界面的交互。使其他人发送的信息能被自己接收。
##### NotifyManager
·改变所有界面的窗口风格、发送其他要求其他窗口改变窗口风格的信号。
##### QClickLabel
·使用QLable标签作为改变皮肤的颜色选择器。使用户点击QLabel标签更换皮肤发出类似PushBtton的点击信号。
##### QMsgTextEdit
·继承自QTextEdit 实现文本发送功能以及发送表情(发送信息部件进行提升，添加对Qt进行网页开发支持)
##### ReceiveFile
·外界发送文件给用户询问是否接收文件。(如果接受文件选择保存位置)
##### RootContatItem
·设置聊天框中分组属性包括箭头等界面设置
##### SendFile
·发送文件给其他用户。(选择要发送的文件)
##### SkinWindow
·实现改变皮肤颜色部件的功能
##### SysTray
·系统托盘的设计
##### TalkWindow
·设计聊天界面及相关功能
##### TalkWindowItem
·聊天的用户头像、名称、关闭按钮显示。
##### TalkWindowShell
·总聊天界面的设计
##### TitleBar
·标题栏头文件
##### UserLogin
·窗口登录界面设计，实现登录账号功能。
##### WindowManager
·添加聊天的用户
