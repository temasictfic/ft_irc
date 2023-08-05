import hexchat
import os
import requests

# Retrieve OpenAI API key from environment variable
OPENAI_API_KEY = os.environ.get("OPENAI_API_KEY")

# Check if the API key is set
if OPENAI_API_KEY is None:
    raise ValueError("Please set the OPENAI_API_KEY environment variable.")

# Constants
BOT_NICK = "IrcGPT"
CHANNEL_NAME = "#wez"
API_ENDPOINT = "https://api.openai.com/v1/engines/gpt-3.5-turbo/completions"

def on_channel_message(word, word_eol, userdata):
    message = word_eol[0]

    # Ignore the message if it's from the bot itself
    if word[0] == BOT_NICK:
        return hexchat.EAT_NONE

    # Check if the message is directed to the bot
    if message.startswith(f"{BOT_NICK}:"):
        user_message = message[len(BOT_NICK) + 1:].strip()
        bot_response = get_bot_response(user_message)

        # Send the response to the channel
        hexchat.command(f"msg {CHANNEL_NAME} {BOT_NICK}: {bot_response}")

    return hexchat.EAT_NONE

def get_bot_response(user_message):
    headers = {
        "Content-Type": "application/json",
        "Authorization": f"Bearer {OPENAI_API_KEY}"
    }

    data = {
        "prompt": user_message,
        "temperature": 0.7,
        "max_tokens": 100
    }

    response = requests.post(API_ENDPOINT, json=data, headers=headers)

    if response.status_code == 200:
        return response.json()["choices"][0]["text"].strip()
    else:
        return "Sorry, there was an error processing your request."

def main():
    hexchat.hook_print("Channel Message", on_channel_message)
    hexchat.print(f"Bot {BOT_NICK} is running.")
    hexchat.command(f"join {CHANNEL_NAME}")

if __name__ == "__main__":
    main()
