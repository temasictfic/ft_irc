
# Server
Servers SHOULD pick a name which contains a dot character (".", 0x2E). This can help clients disambiguate between server names and nicknames in a message source.

# Clients
Nicknames are non-empty strings with the following restrictions:

They MUST NOT contain any of the following characters: space (' ', 0x20), comma (',', 0x2C), asterisk ('*', 0x2A), question mark ('?', 0x3F), exclamation mark ('!', 0x21), at sign ('@', 0x40).
They MUST NOT start with any of the following characters: dollar ('$', 0x24), colon (':', 0x3A).
They MUST NOT start with a character listed as a channel type prefix.
They SHOULD NOT contain any dot character ('.', 0x2E).

# Channels
Channel names are strings (beginning with specified prefix characters). Apart from the requirement of the first character being a valid channel type prefix character; the only restriction on a channel name is that it may not contain any spaces (' ', 0x20), a control G / BELL ('^G', 0x07), or a comma (',', 0x2C) (which is used as a list item separator by the protocol).

There are several types of channels used in the IRC protocol. The first standard type of channel is a regular channel, which is known to all servers that are connected to the network. The prefix character for this type of channel is ('#', 0x23). The second type are server-specific or local channels, where the clients connected can only see and talk to other clients on the same server. The prefix character for this type of channel is ('&', 0x26). Other types of channels are described in the Channel Types section.

Along with various channel types, there are also channel modes that can alter the characteristics and behaviour of individual channels. See the Channel Modes section for more information on these.

To create a new channel or become part of an existing channel, a user is required to join the channel using the JOIN command. If the channel doesn’t exist prior to joining, the channel is created and the creating user becomes a channel operator. If the channel already exists, whether or not the client successfully joins that channel depends on the modes currently set on the channel. For example, if the channel is set to invite-only mode (+i), the client only joins the channel if they have been invited by another user or they have been exempted from requiring an invite by the channel operators.

Channels also contain a topic. The topic is a line shown to all users when they join the channel, and all users in the channel are notified when the topic of a channel is changed. Channel topics commonly state channel rules, links, quotes from channel members, a general description of the channel, or whatever the channel operators want to share with the clients in their channel.

A user may be joined to several channels at once, but a limit may be imposed by the server as to how many channels a client can be in at one time. This limit is specified by the CHANLIMIT RPL_ISUPPORT parameter. See the Feature Advertisement section for more details on RPL_ISUPPORT.

# Channel Operators
Channel operators (or “chanops”) on a given channel are considered to ‘run’ or ‘own’ that channel.

IRC servers may also define other levels of channel moderation. These can include ‘halfop’ (half operator), ‘protected’ (protected user/operator), ‘founder’ (channel founder), and any other positions the server wishes to define. These moderation levels have varying privileges and can execute, and not execute, various channel management commands based on what the server defines.

The commands which may only be used by channel moderators include:

> KICK: Eject a client from the channel
> MODE: Change the channel’s modes
> INVITE: Invite a client to an invite-only channel (mode +i)
> TOPIC: Change the channel topic in a mode +t channel
Channel moderators are identified by the channel member prefix ('@' for standard channel operators, '%' for halfops) next to their nickname whenever it is associated with a channel (e.g. replies to the NAMES, WHO, and WHOIS commands).

Specific prefixes and moderation levels are covered in the Channel Membership Prefixes section.

# Connection Setup
IRC client-server connections work over TCP/IP. The standard ports for client-server connections are TCP/6667 for plaintext, and TCP/6697 for TLS connections.

# Client-to-Server Protocol Structure
While a client is connected to a server, they send a stream of bytes to each other. This stream contains messages separated by CR ('\r', 0x0D) and LF ('\n', 0x0A). These messages may be sent at any time from either side, and may generate zero or more reply messages.

Software SHOULD use the UTF-8 character encoding to encode and decode messages, with fallbacks as described in the Character Encodings implementation considerations appendix.

Names of IRC entities (clients, servers, channels) are casemapped. This prevents, for example, someone having the nickname 'Dan' and someone else having the nickname 'dan', confusing other users. Servers MUST advertise the casemapping they use in the RPL_ISUPPORT numeric that’s sent when connection registration has completed.

# Message Format
An IRC message is a single line, delimited by a pair of CR ('\r', 0x0D) and LF ('\n', 0x0A) characters.

When reading messages from a stream, read the incoming data into a buffer. Only parse and process a message once you encounter the \r\n at the end of it. If you encounter an empty message, silently ignore it.
When sending messages, ensure that a pair of \r\n characters follows every single message your software sends out.

# Source
  > source          ::=  <servername> / ( <nickname> [ "!" <user> ] [ "@" <host> ] )
  > nick            ::=  <any characters except NUL, CR, LF, chantype character, and SPACE> <possibly empty sequence of any characters except NUL, CR, LF, and SPACE>
  > user            ::=  <sequence of any characters except NUL, CR, LF, and SPACE>
The source (formerly known as prefix) is optional and starts with a (':', 0x3A) character (which is stripped from the value), and if there are no tags it MUST be the first character of the message itself.

The source indicates the true origin of a message. If the source is missing from a message, it’s is assumed to have originated from the client/server on the other end of the connection the message was received on.

Clients MUST NOT include a source when sending a message.

Servers MAY include a source on any message, and MAY leave a source off of any message. Clients MUST be able to process any given message the same way whether it contains a source or does not contain one.

# Command
  > command         ::=  letter* / 3digit
The command must either be a valid IRC command or a numeric (a three-digit number represented as text).

Information on specific commands / numerics can be found in the Client Messages and Numerics sections, respectively.

# Parameters
This is the format of the parameters part:

  > parameter       ::=  *( SPACE middle ) [ SPACE ":" trailing ]
  > nospcrlfcl      ::=  <sequence of any characters except NUL, CR, LF, colon (`:`) and SPACE>
  > middle          ::=  nospcrlfcl *( ":" / nospcrlfcl )
  > trailing        ::=  *( ":" / " " / nospcrlfcl )
Parameters (or ‘params’) are extra pieces of information added to the end of a message. These parameters generally make up the ‘data’ portion of the message. What specific parameters mean changes for every single message.

Parameters are a series of values separated by one or more ASCII SPACE characters (' ', 0x20). However, this syntax is insufficient in two cases: a parameter that contains one or more spaces, and an empty parameter. To permit such parameters, the final parameter can be prepended with a (':', 0x3A) character, in which case that character is stripped and the rest of the message is treated as the final parameter, including any spaces it contains. Parameters that contain spaces, are empty, or begin with a ':' character MUST be sent with a preceding ':'; in other cases the use of a preceding ':' on the final parameter is OPTIONAL.

Software SHOULD AVOID sending more than 15 parameters, as older client protocol documents specified this was the maximum and some clients may have trouble reading more than this. However, clients MUST parse incoming messages with any number of them.

Here are some examples of messages and how the parameters would be represented as JSON lists:

  > :irc.example.com CAP * LIST :         ->  ["*", "LIST", ""]

  > CAP * LS :multi-prefix sasl           ->  ["*", "LS", "multi-prefix sasl"]

  > CAP REQ :sasl message-tags foo        ->  ["REQ", "sasl message-tags foo"]

  > :dan!d@localhost PRIVMSG #chan :Hey!  ->  ["#chan", "Hey!"]

  > :dan!d@localhost PRIVMSG #chan Hey!   ->  ["#chan", "Hey!"]

  > :dan!d@localhost PRIVMSG #chan ::-)   ->  ["#chan", ":-)"]
As these examples show, a trailing parameter (a final parameter with a preceding ':') has the same semantics as any other parameter, and MUST NOT be treated specially or stored separately once the ':' is stripped.

# Compatibility with incorrect software
Servers SHOULD handle single \n character, and MAY handle a single \r character, as if it was a \r\n pair, to support existing clients that might send this. However, clients and servers alike MUST NOT send single \r or \n characters.

Servers and clients SHOULD ignore empty lines.

Servers SHOULD gracefully handle messages over the 512-bytes limit. They may:

Send an error numeric back, preferably ERR_INPUTTOOLONG (417)
Truncate on the 510th byte (and add \r\n at the end) or, preferably, on the last UTF-8 character or grapheme that fits.
Ignore the message or close the connection – but this may be confusing to users of buggy clients.
Finally, clients and servers SHOULD NOT use more than one space (\x20) character as SPACE as defined in the grammar above.

# Numeric Replies
Most messages sent from a client to a server generates a reply of some sort. The most common form of reply is the numeric reply, used for both errors and normal replies. Distinct from a normal message, a numeric reply MUST contain a <source> and use a three-digit numeric as the command. A numeric reply SHOULD contain the target of the reply as the first parameter of the message. A numeric reply is not allowed to originate from a client.

In all other respects, a numeric reply is just like a normal message. A list of numeric replies is supplied in the Numerics section.

# Connection Registration
Immediately upon establishing a connection the client must attempt registration, without waiting for any banner message from the server.

Until registration is complete, only a limited subset of commands SHOULD be accepted by the server. This is because it makes sense to require a registered (fully connected) client connection before allowing commands such as JOIN, PRIVMSG and others.

The recommended order of commands during registration is as follows:

> CAP LS 302
> PASS
> NICK and USER
> Capability Negotiation
> SASL (if negotiated)
> CAP END
The commands specified in steps 1-3 should be sent on connection. If the server supports capability negotiation then registration will be suspended and the client can negotiate client capabilities (steps 4-6). If the server does not support capability negotiation then registration will continue immediately without steps 4-6.

If the server supports capability negotiation, the CAP command suspends the registration process and immediately starts the capability negotiation process. CAP LS 302 means that the client supports version 302 of client capability negotiation. The registration process is resumed when the client sends CAP END to the server.

The PASS command is not required for the connection to be registered, but if included it MUST precede the latter of the NICK and USER commands.

The NICK and USER commands are used to set the user’s nickname, username and “real name”. Unless the registration is suspended by a CAP negotiation, these commands will end the registration process.

The client should request advertised capabilities it wishes to enable here.

If the client supports SASL authentication and wishes to authenticate with the server, it should attempt this after a successful CAP ACK of the sasl capability is received and while registration is suspended.

If the server support capability negotiation, CAP END will end the negotiation period and resume the registration.

If the server is waiting to complete a lookup of client information (such as hostname or ident for a username), there may be an arbitrary wait at some point during registration. Servers SHOULD set a reasonable timeout for these lookups.

Additionally, some servers also send a PING and require a matching PONG from the client before continuing. This exchange may happen immediately on connection and at any time during connection registration, so clients MUST respond correctly to it.

Upon successful completion of the registration process, the server MUST send, in this order, the RPL_WELCOME (001), RPL_YOURHOST (002), RPL_CREATED (003), RPL_MYINFO (004), and at least one RPL_ISUPPORT (005) numeric to the client. The server SHOULD then respond as though the client sent the LUSERS command and return the appropriate numerics. If the user has client modes set on them automatically upon joining the network, the server SHOULD send the client the RPL_UMODEIS (221) reply or a MODE message with the client as target, preferably the former. The server MAY send other numerics and messages. The server MUST then respond as though the client sent it the MOTD command, i.e. it must send either the successful Message of the Day numerics or the ERR_NOMOTD (422) numeric.

The first parameter of the RPL_WELCOME (001) message is the nickname assigned by the network to the client. Since it may differ from the nickname the client requested with the NICK command (due to, e.g. length limits or policy restrictions on nicknames), the client SHOULD use this parameter to determine its actual nickname at the time of connection. Subsequent nickname changes, client-initiated or not, will be communicated by the server sending a NICK message.

# Capability Negotiation
Capability Negotiation is a mechanism for the negotiation of protocol extensions, known as client capabilities, that makes sure servers implementing backwards-incompatible protocol extensions still interoperate with existing clients, and vice-versa.

Clients implementing capability negotiation will still interoperate with servers that do not implement it; similarly, servers that implement capability negotiation will successfully communicate with clients that do not implement it.

IRC is an asynchronous protocol, which means that clients may issue additional IRC commands while previous commands are being processed. Additionally, there is no guarantee of a specific kind of banner being issued upon connection. Some servers also do not complain about unknown commands during registration, which means that a client cannot reliably do passive implementation discovery at registration time.

The solution to these problems is to allow for active capability negotiation, and to extend the registration process with this negotiation. If the server supports capability negotiation, the registration process will be suspended until negotiation is completed. If the server does not support this, then registration will complete immediately and the client will not use any capabilities.

Capability negotiation is started by the client issuing a CAP LS 302 command (indicating to the server support for IRCv3.2 capability negotiation). Negotiation is then performed with the CAP REQ, CAP ACK, and CAP NAK commands, and is ended with the CAP END command.

If used during initial registration, and the server supports capability negotiation, the CAP command will suspend registration. Once capability negotiation has ended the registration process will continue.

# Client Messages
In message descriptions, ‘command’ refers to the message’s behaviour when sent from a client to the server. Similarly, ‘Command Examples’ represent example messages sent from a client to the server, and ‘Message Examples’ represent example messages sent from the server to a client. If a command is sent from a client to a server with less parameters than the command requires to be processed, the server will reply with an ERR_NEEDMOREPARAMS (461) numeric and the command will fail.

In the "Parameters:" section, optional parts or parameters are noted with square brackets as such: "[<param>]". Curly braces around a part of parameter indicate that it may be repeated zero or more times, for example: "<key>{,<key>}" indicates that there must be at least one <key>, and that there may be additional keys separated by the comma (",", 0x2C) character.

## PASS message
     Command: PASS
  Parameters: <password>
The PASS command is used to set a ‘connection password’. If set, the password must be set before any attempt to register the connection is made. This requires that clients send a PASS command before sending the NICK / USER combination.

The password supplied must match the one defined in the server configuration. It is possible to send multiple PASS commands before registering but only the last one sent is used for verification and it may not be changed once the client has been registered.

If the password supplied does not match the password expected by the server, then the server SHOULD send ERR_PASSWDMISMATCH (464) and MAY then close the connection with ERROR. Servers MUST send at least one of these two messages.

Servers may also consider requiring SASL authentication upon connection as an alternative to this, when more information or an alternate form of identity verification is desired.

Numeric replies:

> ERR_NEEDMOREPARAMS (461)
> ERR_ALREADYREGISTERED (462)
> ERR_PASSWDMISMATCH (464)
Command Example:

  > PASS secretpasswordhere

## NICK message
     Command: NICK
  Parameters: <nickname>
The NICK command is used to give the client a nickname or change the previous one.

If the server receives a NICK command from a client where the desired nickname is already in use on the network, it should issue an ERR_NICKNAMEINUSE numeric and ignore the NICK command.

If the server does not accept the new nickname supplied by the client as valid (for instance, due to containing invalid characters), it should issue an ERR_ERRONEUSNICKNAME numeric and ignore the NICK command. Servers MUST allow at least all alphanumerical characters, square and curly brackets ([]{}), backslashes (\), and pipe (|) characters in nicknames, and MAY disallow digits as the first character. Servers MAY allow extra characters, as long as they do not introduce ambiguity in other commands, including:

> no leading # character or other character advertized in CHANTYPES
> no leading colon (:)
> no ASCII space
If the server does not receive the <nickname> parameter with the NICK command, it should issue an ERR_NONICKNAMEGIVEN numeric and ignore the NICK command.

The NICK message may be sent from the server to clients to acknowledge their NICK command was successful, and to inform other clients about the change of nickname. In these cases, the <source> of the message will be the old nickname [ [ "!" user ] "@" host ] of the user who is changing their nickname.

Numeric Replies:

> ERR_NONICKNAMEGIVEN (431)
> ERR_ERRONEUSNICKNAME (432)
> ERR_NICKNAMEINUSE (433)
> ERR_NICKCOLLISION (436)
Command Example:

  > NICK Wiz                  ; Requesting the new nick "Wiz".
Message Examples:

  > :WiZ NICK Kilroy          ; WiZ changed his nickname to Kilroy.

  > :dan-!d@localhost NICK Mamoped
                            ; dan- changed his nickname to Mamoped.

## USER message
     Command: USER
  Parameters: <username> 0 * <realname>
The USER command is used at the beginning of a connection to specify the username and realname of a new user.

It must be noted that <realname> must be the last parameter because it may contain SPACE (' ', 0x20) characters, and should be prefixed with a colon (:) if required.

Servers MAY use the Ident Protocol to look up the ‘real username’ of clients. If username lookups are enabled and a client does not have an Identity Server enabled, the username provided by the client SHOULD be prefixed by a tilde ('~', 0x7E) to show that this value is user-set.

The maximum length of <username> may be specified by the USERLEN RPL_ISUPPORT parameter. If this length is advertised, the username MUST be silently truncated to the given length before being used. The minimum length of <username> is 1, ie. it MUST NOT be empty. If it is empty, the server SHOULD reject the command with ERR_NEEDMOREPARAMS (even if an empty parameter is provided); otherwise it MUST use a default value instead.

The second and third parameters of this command SHOULD be sent as one zero ('0', 0x30) and one asterisk character ('*', 0x2A) by the client, as the meaning of these two parameters varies between different versions of the IRC protocol.

Clients SHOULD use the nickname as a fallback value for <username> and <realname> when they don’t have a meaningful value to use.

If a client tries to send the USER command after they have already completed registration with the server, the ERR_ALREADYREGISTERED reply should be sent and the attempt should fail.

If the client sends a USER command after the server has successfully received a username using the Ident Protocol, the <username> parameter from this command should be ignored in favour of the one received from the identity server.

Numeric Replies:

> ERR_NEEDMOREPARAMS (461)
> ERR_ALREADYREGISTERED (462)
Command Examples:

  > USER guest 0 * :Ronnie Reagan
                              > ; No ident server
                              > ; User gets registered with username
                              > "~guest" and real name "Ronnie Reagan"

  > USER guest 0 * :Ronnie Reagan
                              > ; Ident server gets contacted and
                              > returns the name "danp"
                              > ; User gets registered with username
                              > "danp" and real name "Ronnie Reagan"

## PING message
     Command: PING
  Parameters: <token>
The PING command is sent by either clients or servers to check the other side of the connection is still connected and/or to check for connection latency, at the application layer.

The <token> may be any non-empty string.

When receiving a PING message, clients or servers must reply to it with a PONG message with the same <token> value. This allows either to match PONG with the PING they reply to, for example to compute latency.

Clients should not send PING during connection registration, though servers may accept it. Servers may send PING during connection registration and clients must reply to them.

Older versions of the protocol gave specific semantics to the <token> and allowed an extra parameter; but these features are not consistently implemented and should not be relied on. Instead, the <token> should be treated as an opaque value by the receiver.

Numeric Replies:

ERR_NEEDMOREPARAMS (461)
ERR_NOORIGIN (409)
Deprecated Numeric Reply:

ERR_NOSUCHSERVER (402)

## PONG message
     Command: PONG
  Parameters: [<server>] <token>
The PONG command is used as a reply to PING commands, by both clients and servers. The <token> should be the same as the one in the PING message that triggered this PONG.

Servers MUST send a <server> parameter, and clients SHOULD ignore it. It exists for historical reasons, and indicates the name of the server sending the PONG. Clients MUST NOT send a <server> parameter.

Numeric Replies:

None

## QUIT message
    Command: QUIT
 Parameters: [<reason>]
The QUIT command is used to terminate a client’s connection to the server. The server acknowledges this by replying with an ERROR message and closing the connection to the client.

This message may also be sent from the server to a client to show that a client has exited from the network. This is typically only dispatched to clients that share a channel with the exiting user. When the QUIT message is sent to clients, <source> represents the client that has exited the network.

When connections are terminated by a client-sent QUIT command, servers SHOULD prepend <reason> with the ASCII string "Quit: " when sending QUIT messages to other clients, to represent that this user terminated the connection themselves. This applies even if <reason> is empty, in which case the reason sent to other clients SHOULD be just this "Quit: " string. However, clients SHOULD NOT change behaviour based on the prefix of QUIT message reasons, as this is not required behaviour from servers.

If a client connection is closed without the client issuing a QUIT command to the server, the server MUST distribute a QUIT message to other clients informing them of this, distributed in the same was an ordinary QUIT message. Servers MUST fill <reason> with a message reflecting the nature of the event which caused it to happen. For instance, "Ping timeout: 120 seconds", "Excess Flood", and "Too many connections from this IP" are examples of relevant reasons for closing or for a connection with a client to have been closed.

Numeric Replies:

None
Command Example:

  QUIT :Gone to have lunch         ; Client exiting from the network
Message Example:

  :dan-!d@localhost QUIT :Quit: Bye for now!
                                   ; dan- is exiting the network with
                                   the message: "Quit: Bye for now!"


# Channel Operations
This group of messages is concerned with manipulating channels, their properties (channel modes), and their contents (typically clients).

These commands may be requests to the server, in which case the server will or will not grant the request. If a ‘request’ is granted, it will be acknowledged by the server sending a message containing the same information back to the client. This is to tell the user that the request was successful. These sort of ‘request’ commands will be noted in the message information.

## JOIN message
     Command: JOIN
  Parameters: <channel>{,<channel>} [<key>{,<key>}]
  Alt Params: 0
The JOIN command indicates that the client wants to join the given channel(s), each channel using the given key for it. The server receiving the command checks whether or not the client can join the given channel, and processes the request. Servers MUST process the parameters of this command as lists on incoming commands from clients, with the first <key> being used for the first <channel>, the second <key> being used for the second <channel>, etc.

While a client is joined to a channel, they receive all relevant information about that channel including the JOIN, PART, KICK, and MODE messages affecting the channel. They receive all PRIVMSG and NOTICE messages sent to the channel, and they also receive QUIT messages from other clients joined to the same channel (to let them know those users have left the channel and the network). This allows them to keep track of other channel members and channel modes.

If a client’s JOIN command to the server is successful, the server MUST send, in this order:

A JOIN message with the client as the message <source> and the channel they have joined as the first parameter of the message.
The channel’s topic (with RPL_TOPIC (332) and optionally RPL_TOPICWHOTIME (333)), and no message if the channel does not have a topic.
A list of users currently joined to the channel (with one or more RPL_NAMREPLY (353) numerics followed by a single RPL_ENDOFNAMES (366) numeric). These RPL_NAMREPLY messages sent by the server MUST include the requesting client that has just joined the channel.
The key, client limit , ban - exception, invite-only - exception, and other (depending on server software) channel modes affect whether or not a given client may join a channel. More information on each of these modes and how they affect the JOIN command is available in their respective sections.

Servers MAY restrict the number of channels a client may be joined to at one time. This limit SHOULD be defined in the CHANLIMIT RPL_ISUPPORT parameter. If the client cannot join this channel because they would be over their limit, they will receive an ERR_TOOMANYCHANNELS (405) reply and the command will fail.

Note that this command also accepts the special argument of ("0", 0x30) instead of any of the usual parameters, which requests that the sending client leave all channels they are currently connected to. The server will process this command as though the client had sent a PART command for each channel they are a member of.

This message may be sent from a server to a client to notify the client that someone has joined a channel. In this case, the message <source> will be the client who is joining, and <channel> will be the channel which that client has joined. Servers SHOULD NOT send multiple channels in this message to clients, and SHOULD distribute these multiple-channel JOIN messages as a series of messages with a single channel name on each.

Numeric Replies:

ERR_NEEDMOREPARAMS (461)
ERR_NOSUCHCHANNEL (403)
ERR_TOOMANYCHANNELS (405)
ERR_BADCHANNELKEY (475)
ERR_BANNEDFROMCHAN (474)
ERR_CHANNELISFULL (471)
ERR_INVITEONLYCHAN (473)
ERR_BADCHANMASK (476)
RPL_TOPIC (332)
RPL_TOPICWHOTIME (333)
RPL_NAMREPLY (353)
RPL_ENDOFNAMES (366)
Command Examples:

  JOIN #foobar                    ; join channel #foobar.

  JOIN &foo fubar                 ; join channel &foo using key "fubar".

  JOIN #foo,&bar fubar            ; join channel #foo using key "fubar"
                                  and &bar using no key.

  JOIN #foo,#bar fubar,foobar     ; join channel #foo using key "fubar".
                                  and channel #bar using key "foobar".

  JOIN #foo,#bar                  ; join channels #foo and #bar.
Message Examples:

  :WiZ JOIN #Twilight_zone        ; WiZ is joining the channel
                                  #Twilight_zone

  :dan-!d@localhost JOIN #test    ; dan- is joining the channel #test

## PART message
     Command: PART
  Parameters: <channel>{,<channel>} [<reason>]
The PART command removes the client from the given channel(s). On sending a successful PART command, the user will receive a PART message from the server for each channel they have been removed from. <reason> is the reason that the client has left the channel(s).

For each channel in the parameter of this command, if the channel exists and the client is not joined to it, they will receive an ERR_NOTONCHANNEL (442) reply and that channel will be ignored. If the channel does not exist, the client will receive an ERR_NOSUCHCHANNEL (403) reply and that channel will be ignored.

This message may be sent from a server to a client to notify the client that someone has been removed from a channel. In this case, the message <source> will be the client who is being removed, and <channel> will be the channel which that client has been removed from. Servers SHOULD NOT send multiple channels in this message to clients, and SHOULD distribute these multiple-channel PART messages as a series of messages with a single channel name on each. If a PART message is distributed in this way, <reason> (if it exists) should be on each of these messages.

Numeric Replies:

ERR_NEEDMOREPARAMS (461)
ERR_NOSUCHCHANNEL (403)
ERR_NOTONCHANNEL (442)
Command Examples:

  PART #twilight_zone             ; leave channel "#twilight_zone"

  PART #oz-ops,&group5            ; leave both channels "&group5" and
                                  "#oz-ops".
Message Examples:

  :dan-!d@localhost PART #test    ; dan- is leaving the channel #test

## TOPIC message
     Command: TOPIC
  Parameters: <channel> [<topic>]
The TOPIC command is used to change or view the topic of the given channel. If <topic> is not given, either RPL_TOPIC or RPL_NOTOPIC is returned specifying the current channel topic or lack of one. If <topic> is an empty string, the topic for the channel will be cleared.

If the client sending this command is not joined to the given channel, and tries to view its’ topic, the server MAY return the ERR_NOTONCHANNEL (442) numeric and have the command fail.

If RPL_TOPIC is returned to the client sending this command, RPL_TOPICWHOTIME SHOULD also be sent to that client.

If the protected topic mode is set on a channel, then clients MUST have appropriate channel permissions to modify the topic of that channel. If a client does not have appropriate channel permissions and tries to change the topic, the ERR_CHANOPRIVSNEEDED (482) numeric is returned and the command will fail.

If the topic of a channel is changed or cleared, every client in that channel (including the author of the topic change) will receive a TOPIC command with the new topic as argument (or an empty argument if the topic was cleared) alerting them to how the topic has changed.

Clients joining the channel in the future will receive a RPL_TOPIC numeric (or lack thereof) accordingly.

Numeric Replies:

ERR_NEEDMOREPARAMS (461)
ERR_NOSUCHCHANNEL (403)
ERR_NOTONCHANNEL (442)
ERR_CHANOPRIVSNEEDED (482)
RPL_NOTOPIC (331)
RPL_TOPIC (332)
RPL_TOPICWHOTIME (333)
Command Examples:

  TOPIC #test :New topic          ; Setting the topic on "#test" to
                                  "New topic".

  TOPIC #test :                   ; Clearing the topic on "#test"

  TOPIC #test                     ; Checking the topic for "#test"

## NAMES message
     Command: NAMES
  Parameters: <channel>{,<channel>}
The NAMES command is used to view the nicknames joined to a channel and their channel membership prefixes. The param of this command is a list of channel names, delimited by a comma (",", 0x2C) character.

The channel names are evaluated one-by-one. For each channel that exists and they are able to see the users in, the server returns one of more RPL_NAMREPLY numerics containing the users joined to the channel and a single RPL_ENDOFNAMES numeric. If the channel name is invalid or the channel does not exist, one RPL_ENDOFNAMES numeric containing the given channel name should be returned. If the given channel has the secret channel mode set and the user is not joined to that channel, one RPL_ENDOFNAMES numeric is returned. Users with the invisible user mode set are not shown in channel responses unless the requesting client is also joined to that channel.

Servers MAY allow more than one target channel. They can advertise the maximum the number of target users per NAMES command via the TARGMAX RPL_ISUPPORT parameter.

Numeric Replies:

RPL_NAMREPLY (353)
RPL_ENDOFNAMES (366)
Command Examples:

  NAMES #twilight_zone,#42        ; List all visible users on
                                  "#twilight_zone" and "#42".

  NAMES                           ; Attempt to list all visible users on
                                  the network, which SHOULD be responded to
                                  as specified above.

## LIST message
     Command: LIST
  Parameters: [<channel>{,<channel>}] [<elistcond>{,<elistcond>}]
The LIST command is used to get a list of channels along with some information about each channel. Both parameters to this command are optional as they have different syntaxes.

The first possible parameter to this command is a list of channel names, delimited by a comma (",", 0x2C) character. If this parameter is given, the information for only the given channels is returned. If this parameter is not given, the information about all visible channels (those not hidden by the secret channel mode rules) is returned.

In response to a successful LIST command, the server MAY send one RPL_LISTSTART numeric, MUST send back zero or more RPL_LIST numerics, and MUST send back one RPL_LISTEND numeric.

Numeric Replies:

RPL_LISTSTART (321)
RPL_LIST (322)
RPL_LISTEND (323)
Command Examples:

  LIST                            ; Command to list all channels

  LIST #twilight_zone,#42         ; Command to list the channels
                                  "#twilight_zone" and "#42".

## INVITE message
     Command: INVITE
  Parameters: <nickname> <channel>
The INVITE command is used to invite a user to a channel. The parameter <nickname> is the nickname of the person to be invited to the target channel <channel>.

The target channel SHOULD exist (at least one user is on it). Otherwise, the server SHOULD reject the command with the ERR_NOSUCHCHANNEL numeric.

Only members of the channel are allowed to invite other users. Otherwise, the server MUST reject the command with the ERR_NOTONCHANNEL numeric.

Servers MAY reject the command with the ERR_CHANOPRIVSNEEDED numeric. In particular, they SHOULD reject it when the channel has invite-only mode set, and the user is not a channel operator.

If the user is already on the target channel, the server MUST reject the command with the ERR_USERONCHANNEL numeric.

When the invite is successful, the server MUST send a RPL_INVITING numeric to the command issuer, and an INVITE message, with the issuer as <source>, to the target user. Other channel members SHOULD NOT be notified.

Numeric Replies:

RPL_INVITING (341)
ERR_NEEDMOREPARAMS (461)
ERR_NOSUCHCHANNEL (403)
ERR_NOTONCHANNEL (442)
ERR_CHANOPRIVSNEEDED (482)
ERR_USERONCHANNEL (443)
Command Examples:

  INVITE Wiz #foo_bar    ; Invite Wiz to #foo_bar
Message Examples:

  :dan-!d@localhost INVITE Wiz #test    ; dan- has invited Wiz
                                        to the channel #test

## KICK message
      Command: KICK
   Parameters: <channel> <user> *( "," <user> ) [<comment>]
The KICK command can be used to request the forced removal of a user from a channel. It causes the <user> to be removed from the <channel> by force. If no comment is given, the server SHOULD use a default message instead.

The server MUST NOT send KICK messages with multiple users to clients. This is necessary to maintain backward compatibility with existing client software.

Servers MAY limit the number of target users per KICK command via the TARGMAX parameter of RPL_ISUPPORT, and silently drop targets if the number of targets exceeds the limit.

Numeric Replies:

ERR_NEEDMOREPARAMS (461)
ERR_NOSUCHCHANNEL (403)
ERR_CHANOPRIVSNEEDED (482)
ERR_USERNOTINCHANNEL (441)
ERR_NOTONCHANNEL (442)

Examples:

   KICK #Finnish Matthew           ; Command to kick Matthew from
                                   #Finnish

   KICK &Melbourne Matthew         ; Command to kick Matthew from
                                   &Melbourne

   KICK #Finnish John :Speaking English
                                   ; Command to kick John from #Finnish
                                   using "Speaking English" as the
                                   reason (comment).

   :WiZ!jto@tolsun.oulu.fi KICK #Finnish John
                                   ; KICK message on channel #Finnish
                                   from WiZ to remove John from channel

# Sending Messages
## PRIVMSG message
     Command: PRIVMSG
  Parameters: <target>{,<target>} <text to be sent>
The PRIVMSG command is used to send private messages between users, as well as to send messages to channels. <target> is the nickname of a client or the name of a channel.

If <target> is a channel name and the client is banned and not covered by a ban exception, the message will not be delivered and the command will silently fail. Channels with the moderated mode active may block messages from certain users. Other channel modes may affect the delivery of the message or cause the message to be modified before delivery, and these modes are defined by the server software and configuration being used.

If a message cannot be delivered to a channel, the server SHOULD respond with an ERR_CANNOTSENDTOCHAN (404) numeric to let the user know that this message could not be delivered.

If <target> is a channel name, it may be prefixed with one or more channel membership prefix character (@, +, etc) and the message will be delivered only to the members of that channel with the given or higher status in the channel. Servers that support this feature will list the prefixes which this is supported for in the STATUSMSG RPL_ISUPPORT parameter, and this SHOULD NOT be attempted by clients unless the prefix has been advertised in this token.

If <target> is a user and that user has been set as away, the server may reply with an RPL_AWAY (301) numeric and the command will continue.

The PRIVMSG message is sent from the server to client to deliver a message to that client. The <source> of the message represents the user or server that sent the message, and the <target> represents the target of that PRIVMSG (which may be the client, a channel, etc).

When the PRIVMSG message is sent from a server to a client and <target> starts with a dollar character ('$', 0x24), the message is a broadcast sent to all clients on one or multiple servers.

Numeric Replies:

ERR_NOSUCHNICK (401)
ERR_NOSUCHSERVER (402)
ERR_CANNOTSENDTOCHAN (404)
ERR_TOOMANYTARGETS (407)
ERR_NORECIPIENT (411)
ERR_NOTEXTTOSEND (412)
ERR_NOTOPLEVEL (413)
ERR_WILDTOPLEVEL (414)
RPL_AWAY (301)

Command Examples:

  PRIVMSG Angel :yes I'm receiving it !
                                  ; Command to send a message to Angel.

  PRIVMSG %#bunny :Hi! I have a problem!
                                  ; Command to send a message to halfops
                                  and chanops on #bunny.

  PRIVMSG @%#bunny :Hi! I have a problem!
                                  ; Command to send a message to halfops
                                  and chanops on #bunny. This command is
                                  functionally identical to the above
                                  command.
Message Examples:

  :Angel PRIVMSG Wiz :Hello are you receiving this message ?
                                  ; Message from Angel to Wiz.

  :dan!~h@localhost PRIVMSG #coolpeople :Hi everyone!
                                  ; Message from dan to the channel
                                  #coolpeople

## NOTICE message
     Command: NOTICE
  Parameters: <target>{,<target>} <text to be sent>
The NOTICE command is used to send notices between users, as well as to send notices to channels. <target> is interpreted the same way as it is for the PRIVMSG command.

The NOTICE message is used similarly to PRIVMSG. The difference between NOTICE and PRIVMSG is that automatic replies must never be sent in response to a NOTICE message. This rule also applies to servers – they must not send any error back to the client on receipt of a NOTICE command. The intention of this is to avoid loops between a client automatically sending something in response to something it received. This is typically used by ‘bots’ (a client with a program, and not a user, controlling their actions) and also for server messages to clients.

One thing for bot authors to note is that the NOTICE message may be interpreted differently by various clients. Some clients highlight or interpret any NOTICE sent to a channel in the same way that a PRIVMSG with their nickname gets interpreted. This means that users may be irritated by the use of NOTICE messages rather than PRIVMSG messages by clients or bots, and they are not commonly used by client bots for this reason.

# User-Based Queries
## WHO message
     Command: WHO
  Parameters: <mask>
This command is used to query a list of users who match the provided mask. The server will answer this command with zero, one or more RPL_WHOREPLY, and end the list with RPL_ENDOFWHO.

The mask can be one of the following:

A channel name, in which case the channel members are listed.
An exact nickname, in which case a single user is returned.
A mask pattern, in which case all visible users whose nickname matches are listed. Servers MAY match other user-specific values, such as the hostname, server, real name or username. Servers MAY not support mask patterns and return an empty list.
Visible users are users who aren’t invisible (user mode +i) and who don’t have a common channel with the requesting client. Servers MAY filter or limit visible users replies arbitrarily.

Numeric Replies:

RPL_WHOREPLY (352)
RPL_ENDOFWHO (315)
ERR_NOSUCHSERVER (402)

Examples
Command Examples:

  WHO emersion        ; request information on user "emersion"
  WHO #ircv3          ; list users in the "#ircv3" channel
Reply Examples:

  :calcium.libera.chat 352 dan #ircv3 ~emersion sourcehut/staff/emersion calcium.libera.chat emersion H :1 Simon Ser
  :calcium.libera.chat 315 dan emersion :End of WHO list
                                  ; Reply to WHO emersion

  :calcium.libera.chat 352 dan #ircv3 ~emersion sourcehut/staff/emersion calcium.libera.chat emersion H :1 Simon Ser
  :calcium.libera.chat 352 dan #ircv3 ~val limnoria/val calcium.libera.chat val H :1 Val
  :calcium.libera.chat 315 dan #ircv3 :End of WHO list
                                  ; Reply to WHO #ircv3

# Channel Types
IRC has various types of channels that act in different ways. What differentiates these channels is the character the channel name starts with. For instance, channels starting with # are regular channels, and channels starting with & are local channels.

Upon joining, clients are shown which types of channels the server supports with the CHANTYPES parameter.

Here, we go through the different types of channels that exist and are widely-used these days.

## Regular Channels (#)
The prefix character for this type of channel is ('#', 0x23).

This channel is what’s referred to as a normal channel. Clients can join this channel, and the first client who joins a normal channel is made a channel operator, along with the appropriate channel membership prefix. On most servers, newly-created channels have then protected topic "+t" and no external messages "+n" modes enabled, but exactly what modes new channels are given is up to the server.

Regular channels are persisted across the network. If two clients on different servers join the same regular channel, they’ll be able to see that each other are joined, and will see messages sent to the channel by the other client.

On servers that support the concept of ‘channel ownership’ (a client being able to own a channel and retain control of it with their account), clients may not receive channel operator priveledges on joining an otherwise empty channel.

## Local Channels (&)
The prefix character for this type of channel is ('&', 0x26).

This channel is what’s referred to as a local channel. Clients can join this channel as normal, and the first client who joins a normal channel is made a channel operator, but the channel is not persisted across the network. In other words, each server has its own set of local channels that the other servers on the network don’t see.

If a client on server A and a client on server B join the channel &info, they will not be able to see each other or the messages each posts to their server’s local channel &info. However, if a client on server A and another client on server A join the channel &info, they will be able to see each other and the messages the other posts to that local channel.

Generally, the concept of channel ownership is not supported for local channels. Local channels also aren’t as widely available as regular channels. As well, some networks disable or disallow local channels as opers across the network can’t see nor administrate them.

# User Modes
## Invisible User Mode
This mode is standard, and the mode letter used for it is "+i".

If a user is set to ‘invisible’, they will not show up in commands such as WHO or NAMES unless they share a channel with the user that submitted the command. In addition, some servers hide all channels from the WHOIS reply of an invisible user they do not share with the user that submitted the command.

## Registered User Mode
This mode is widely-used, and the mode letter used for it is typically "+r". The character used for this mode, and whether it exists at all, may vary depending on server software and configuration.

If a user has this mode, this indicates that they have logged into a user account.

# Channel Modes
## Ban Channel Mode
This mode is standard, and the mode letter used for it is "+b".

This channel mode controls a list of client masks that are ‘banned’ from joining or speaking in the channel. If this mode has values, each of these values should be a client mask.

If this mode is set on a channel, and a client sends a JOIN request for this channel, their nickmask (the combination of nick!user@host) is compared with each banned client mask set with this mode. If they match one of these banned masks, they will receive an ERR_BANNEDFROMCHAN (474) reply and the JOIN command will fail. See the ban exception mode for more details.


## Exception Channel Mode
This mode is used in almost all IRC software today. The standard mode letter used for it is "+e", but it SHOULD be defined in the EXCEPTS RPL_ISUPPORT parameter on connection.

This channel mode controls a list of client masks that are exempt from the ‘ban’ channel mode. If this mode has values, each of these values should be a client mask.

If this mode is set on a channel, and a client sends a JOIN request for this channel, their nickmask is compared with each ‘exempted’ client mask. If their nickmask matches any one of the masks set by this mode, and their nickmask also matches any one of the masks set by the ban channel mode, they will not be blocked from joining due to the ban mode.

## Client Limit Channel Mode
This mode is standard, and the mode letter used for it is "+l".

This channel mode controls whether new users may join based on the number of users who already exist in the channel. If this mode is set, its value is an integer and defines the limit of how many clients may be joined to the channel.

If this mode is set on a channel, and the number of users joined to that channel matches or exceeds the value of this mode, new users cannot join that channel. If a client sends a JOIN request for this channel, they will receive an ERR_CHANNELISFULL (471) reply and the command will fail.

## Invite-Only Channel Mode
This mode is standard, and the mode letter used for it is "+i".

This channel mode controls whether new users need to be invited to the channel before being able to join.

If this mode is set on a channel, a user must have received an INVITE for this channel before being allowed to join it. If they have not received an invite, they will receive an ERR_INVITEONLYCHAN (473) reply and the command will fail.

## Invite-Exception Channel Mode
This mode is used in almost all IRC software today. The standard mode letter used for it is "+I", but it SHOULD be defined in the INVEX RPL_ISUPPORT parameter on connection.

This channel mode controls a list of channel masks that are exempt from the invite-only channel mode. If this mode has values, each of these values should be a client mask.

If this mode is set on a channel, and a client sends a JOIN request for that channel, their nickmask is compared with each ‘exempted’ client mask. If their nickmask matches any one of the masks set by this mode, and the channel is in invite-only mode, they do not need to require an INVITE in order to join the channel.

## Key Channel Mode
This mode is standard, and the mode letter used for it is "+k".

This mode letter sets a ‘key’ that must be supplied in order to join this channel. If this mode is set, its’ value is the key that is required. Servers may validate the value (eg. to forbid spaces, as they make it harder to use the key in JOIN messages). If the value is invalid, they SHOULD return ERR_INVALIDMODEPARAM. However, clients MUST be able to handle any of the following:

ERR_INVALIDMODEPARAM
ERR_INVALIDKEY
MODE echoed with a different key (eg. truncated or stripped of invalid characters)
the key changed ignored, and no MODE echoed if no other mode change was valid.
If this mode is set on a channel, and a client sends a JOIN request for that channel, they must supply <key> in order for the command to succeed. If they do not supply a <key>, or the key they supply does not match the value of this mode, they will receive an ERR_BADCHANNELKEY (475) reply and the command will fail.

## Moderated Channel Mode
This mode is standard, and the mode letter used for it is "+m".

This channel mode controls whether users may freely talk on the channel, and does not have any value.

If this mode is set on a channel, only users who have channel privileges may send messages to that channel. The voice channel mode is designed to let a user talk in a moderated channel without giving them other channel moderation abilities, and users of higher privileges (such as halfops or chanops) may also speak in moderated channels.

## Secret Channel Mode
This mode is standard, and the mode letter used for it is "+s".

This channel mode controls whether the channel is ‘secret’, and does not have any value.

A channel that is set to secret will not show up in responses to the LIST or NAMES command unless the client sending the command is joined to the channel. Likewise, secret channels will not show up in the RPL_WHOISCHANNELS (319) numeric unless the user the numeric is being sent to is joined to that channel.

## Protected Topic Mode
This mode is standard, and the mode letter used for it is "+t".

This channel mode controls whether channel privileges are required to set the topic, and does not have any value.

If this mode is enabled, users must have channel privileges such as halfop or operator status in order to change the topic of a channel. In a channel that does not have this mode enabled, anyone may set the topic of the channel using the TOPIC command.

## No External Messages Mode
This mode is standard, and the mode letter used for it is "+n".

This channel mode controls whether users who are not joined to the channel can send messages to it, and does not have any value.

If this mode is enabled, users MUST be joined to the channel in order to send private messages and notices to the channel. If this mode is enabled and they try to send one of these to a channel they are not joined to, they will receive an ERR_CANNOTSENDTOCHAN (404) numeric and the message will not be sent to that channel.

# Channel Membership Prefixes
Users joined to a channel may get certain privileges or status in that channel based on channel modes given to them. These users are given prefixes before their nickname whenever it is associated with a channel (ie, in NAMES, WHO and WHOIS messages). The standard and common prefixes are listed here, and MUST be advertised by the server in the PREFIX RPL_ISUPPORT parameter on connection.

## Founder Prefix
This mode is used in a large number of networks. The prefix and mode letter typically used for it, respectively, are "~" and "+q".

This prefix shows that the given user is the ‘founder’ of the current channel and has full moderation control over it – ie, they are considered to ‘own’ that channel by the network. This prefix is typically only used on networks that have the concept of client accounts, and ownership of channels by those accounts.

## Protected Prefix
This mode is used in a large number of networks. The prefix and mode letter typically used for it, respectively, are "&" and "+a".

Users with this mode cannot be kicked and cannot have this mode removed by other protected users. In some software, they may perform actions that operators can, but at a higher privilege level than operators. This prefix is typically only used on networks that have the concept of client accounts, and ownership of channels by those accounts.

## Operator Prefix
This mode is standard. The prefix and mode letter used for it, respectively, are "@" and "+o".

Users with this mode may perform channel moderation tasks such as kicking users, applying channel modes, and set other users to operator (or lower) status.

## Halfop Prefix
This mode is widely used in networks today. The prefix and mode letter used for it, respectively, are "%" and "+h".

Users with this mode may perform channel moderation tasks, but at a lower privilege level than operators. Which channel moderation tasks they can and cannot perform varies with server software and configuration.

## Voice Prefix
This mode is standard. The prefix and mode letter used for it, respectively, are "+" and "+v".

Users with this mode may send messages to a channel that is moderated.

[RPL_](https://modern.ircdocs.horse/#numerics)
