后端和数据库api

#### 登录（检验密码用户名正确）

bool login(usr, password, &user_Info) 

入参

usr：用户名 string

password：密码 string

&user_Info：用户信息地址 class user_Info

出参

success



#### 消息请求（处理各种类型请求：发送消息、文件、图片）

bool newMsg(msg)

入参

msg：消息请求 class Message

出参

success



#### 注册用户

bool register(log_name, password)

入参

log_name：用户名

password：密码

出参

id  

0:fail

other:success



#### 新建聊天

int newGroup(kind, groupName)

入参

kind：标识是人还是群组

0：人

1：群

groupName：群组名字

出参

groupID

0：fail

other：success



#### 成员加入群聊

bool addMember(groupID, usrID) 

入参

groupID：群组号

usrID：用户号

出参

successs



#### 成员退出群聊

bool deleteMember(groupID, usrID)

入参

groupID：群组号

usrID：用户号

出参

successs



#### 删除群组

bool deleteGroup(groupID, usrID)

入参

groupID：群组号

usrID：发起操作人的用户号

出参

success



#### 群成员列表

bool groupMembers(groupID,  &group_members)

入参

groupID：群组号

group_members：群成员数组引用

出参

success



#### 好友列表

bool friendlist(usrID,  &friendlist)

入参

usrID：用户号

friendlist：好友列表

出参

success



#### 更改个人信息

bool edit_usr_info(usrID, usrinfo)

入参

usrID：用户id

usrinfo：新用户信息

出参

success



#### 获取个人信息

bool get_usr_info(usrIist, usrinfo)

入参

usrID：用户id列表

usrinfo：用户信息引用

出参

success



#### 获取消息记录

bool get_history(limit, usrID, messageData)

入参

limit 查询条数

usrID 用户id

messageData 消息记录数组

出参

success



