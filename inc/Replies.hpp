#pragma once


#define RPL_WELCOME(Nick, UserName) ":ircserv 001 " + Nick + "@localhost :Welcome to ircserv made by Ataskin and Sciftci, " + Nick + "!" + UserName + "@localhost" 

#define TOKENS "CASEMAPPING=ascii, CHANLIMIT=#:4, CHANMODES=b,i,k,l,t, PREFIX=(o)@, TARGMAX=NAMES:1,LIST:1,KICK:1,JOIN:1,PRIVMSG:1,NOTICE:1, TOPICLEN=254"

#define RPL_ISUPPORT(Nick)  ":ircserv 005 " + Nick + "@localhost " + TOKENS + " :are supported by this server"

//RPL_LISTSTART (321) "<client> Channel :Users  Name"

#define RPL_LIST(Nick, ChanName, ChanCount, Topic) ":ircserv 322 " + Nick + "@localhost " + ChanName + " " + ChanCount + " :" + Topic

#define RPL_LISTEND(Nick) ":ircserv 323 " + Nick + "@localhost " + ":End of /LIST"

#define RPL_CHANNELMODEIS(Nick, ChanName, ModeString, ModeArgsList) ":ircserv 324 " + Nick + "@localhost " + ChanName + " " + ModeString + " " + ModeArgsList

#define RPL_NOTOPIC(Nick, ChanName) ":ircserv 331 " + Nick + "@localhost " + ChanName + " :No topic is set"

#define RPL_TOPIC(Nick, ChanName, Topic) ":ircserv 332 " + Nick + "@localhost " + ChanName + " :" + Topic

//#define RPL_TOPICWHOTIME(Nick, ChanName, TopicSetterNick, TimeStamp) ":ircserv 333 " + Nick + "@localhost " + ChanName + " " + TopicSetterNick + " " + TimeStamp

//RPL_INVITELIST (336) "<client> <channel>"

//RPL_ENDOFINVITELIST (337) "<client> :End of /INVITE list"

#define RPL_INVITING(Nick, InvitedNick, ChanName) ":ircserv 341 " + Nick + "@localhost " + InvitedNick + " " + ChanName

#define RPL_NAMREPLY(Nick, ChanName, PrefixNickList) ":ircserv 353 " + Nick + "@localhost = " + ChanName + " :" + PrefixNickList

#define RPL_ENDOFNAMES(Nick, ChanName) ":ircserv 366 " + Nick + "@localhost " + ChanName + " :End of /NAMES list"

#define RPL_BANLIST(Nick, ChanName, Mask) ":ircserv 367 " + Nick + "@localhost " + ChanName + " " + Mask

#define RPL_ENDOFBANLIST(Nick, ChanName) ":ircserv 368 " + Nick + "@localhost " + ChanName + " :End of channel ban list"

//#define RPL_WHOISMODES(Nicki, Modes) ":ircserv 379 " + Nick + "@localhost :is using modes " + Modes 

//----ERRORS
#define ERR_UNKNOWNERROR(Nick, Command, Message) ":ircserv 400 " + Nick + "@localhost " + Command + " :" + Message

#define ERR_NOSUCHNICK(Nick, Nickname) ":ircserv 401 " + Nick + "@localhost " + Nickname + " :No such nick"

#define ERR_NOSUCHCHANNEL(Nick, ChanName) ":ircserv 403 " + Nick + "@localhost " + ChanName + " :No such channel"

#define ERR_CANNOTSENDTOCHAN(Nick, ChanName) ":ircserv 404 " + Nick + "@localhost " + ChanName +  " :Cannot send to channel"

#define ERR_TOOMANYCHANNELS(Nick, ChanName) ":ircserv 405 " + Nick + "@localhost " + ChanName + " :You have joined too many channels"

#define ERR_NORECIPIENT(Nick, Command) ":ircserv 411 " + Nick + "@localhost " + ":No recipient given (" + Command + ")"

#define ERR_NOTEXTTOSEND(Nick) ":ircserv 412 " + Nick + "@localhost " + ":No text to send"

#define ERR_INPUTTOOLONG(Nick) ":ircserv 417 " + Nick + "@localhost " + ":Input line was too long"

#define ERR_UNKNOWNCOMMAND(Nick, Command) ":ircserv 421 " + Nick + "@localhost " + Command + " :Unknown command"

#define ERR_NONICKNAMEGIVEN(Nick) ":ircserv 431 " + Nick + "@localhost " + ":No nickname given"

#define ERR_ERRONEUSNICKNAME(Nick) ":ircserv 432 " + Nick + "@localhost " + Nick + " :Erroneus nickname"

#define ERR_NICKNAMEINUSE(Nick) ":ircserv 433 " + Nick + "@localhost " + Nick + " :Nickname is already in use"

#define ERR_USERNOTINCHANNEL(Nick, Client, ChanName) ":ircserv 441 " + Nick + "@localhost " + Client + " " + ChanName + " :They aren't on that channel"

#define ERR_NOTONCHANNEL(Nick, ChanName) ":ircserv 442 " + Nick + "@localhost " + Nick + " " + ChanName + " :You're not on that channel"

#define ERR_USERONCHANNEL(Nick, Client, ChanName) ":ircserv 443 " + Nick + "@localhost " + Client + " " + ChanName + " :is already on channel"

#define ERR_NOTREGISTERED(Nick) ":ircserv 451 " + Nick + "@localhost " + ":You have not registered"

#define ERR_NEEDMOREPARAMS(Nick, Command) ":ircserv 461 " + Nick + "@localhost " + Command +  " :Not enough parameters"

#define ERR_ALREADYREGISTERED(Nick) ":ircserv 462 " + Nick + "@localhost " + ":You may not reregister"

#define ERR_PASSWDMISMATCH(Nick) ":ircserv 464 " + Nick + "@localhost " +  ":Password incorrect"

#define ERR_YOUREBANNEDCREEP(Nick) ":ircserv 462 " + Nick + "@localhost " +  ":You banned from this server"

#define ERR_CHANNELISFULL(Nick, ChanName) ":ircserv 471 " + Nick + "@localhost " + ChanName + " :Cannot join channel (+l)"

#define ERR_UNKNOWNMODE(Nick, ModeChar) ":ircserv 472 " + Nick + "@localhost " + ModeChar + " :is unknown mode char to me"

#define ERR_INVITEONLYCHAN(Nick, ChanName) ":ircserv 473 " + Nick + "@localhost " + ChanName + " :Cannot join channel (+i)"

#define ERR_BANNEDFROMCHAN(Nick, ChanName) ":ircserv 474 " + Nick + "@localhost " + ChanName + " :Cannot join channel (+b)"

#define ERR_BADCHANNELKEY(Nick, ChanName) ":ircserv 475 " + Nick + "@localhost " + ChanName + " :Cannot join channel (+k)"

#define ERR_CHANOPRIVSNEEDED(Nick, ChanName) ":ircserv 482 " + Nick + "@localhost " + ChanName + " :You're not channel operator"

#define ERR_UMODEUNKNOWNFLAG(Nick, Modechar) ":ircserv 501 " + Nick + "@localhost " + ModeChar + " :Unknown MODE flag"

#define ERR_INVALIDKEY(Nick, ChanName) ":ircserv 525 " + Nick + "@localhost " + ChanName + " :Key is not well-formed"