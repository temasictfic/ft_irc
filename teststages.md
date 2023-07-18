#Server
- [] Check port number valid or print error.
- [] Check password valid or print error.
- [] Server listening for clients.

#Clients
- [] Client start with **/PASS <password>** command. Check password valid and Client status == PasswordRegistered
- [] Client use **/NICK <nickname>** command. Check nickname valid and Client status == NickRegistered
- [] Client use **/USER <username> [<realname>]** command. Check username and realname valid and
     Client status = UserRegistered
- [] Check if u can use other cmds before UserRegistered status.
- [] Add new client to client list after fully registered and send RPL_WELCOME
- [] Use /LIST **[<#channelname>]** and **/NAMES <#channelname>** cmds see its empty.
- [] Use **/PING** cmd and see "Pong"
- [] Use **/JOIN <#channelname>** see u r operator, sending u joined message and u have operator prefix. U added to 
     member list.
- [] Use **/LIST [<#channelname>]** and **/NAMES <#channelname>** cmds see there is one and info about it.
- [] Use **/MODE <+-i, +-k, +-t>** working properly and sending Modechange message
- [] Use **/INVITE <nickname>** on InviteOnly Mode see only operator invite user and without InviteOnly Mode.
- [] Use **/TOPIC <topic>** on ProtectedTopic Mode see only operator change topic and without ProtectedTopic Mode.
- [] Use **/PASS <password>** on Keyed Mode see only users with password can join the channel.
- [] Use **/KICK <nickname>** to kick user from the channel as operator. that user added to banned list.
     After that use **/INVITE <nickname>** to add that user to channel and remove from banned list.
- [] Use **/PART <reason>** to apart from channel and send channel that user aparted. If u r not in channel u cant use
     this cmd.
- [] Use **/QUIT <reason>** to quit from server.
- [] Use **/NOTICE <bot message>** on channels.
- [] Check reply message for each.

