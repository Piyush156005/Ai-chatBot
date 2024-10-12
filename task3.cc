#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <map>

// Utility function to convert string to lowercase
std::string toLowerCase(const std::string &str)
{
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// Class to handle chatbot memory (basic context awareness)
class ChatbotMemory
{
public:
    std::string lastSentiment = "neutral"; // To remember the last sentiment detected
    std::string lastTopic = "";            // To remember the topic discussed
};

// Simple random response generator
std::pair<int, std::string> analyzeSentiment(const std::string &input)
{
    std::vector<std::string> positiveWords = {"happy", "great", "good", "awesome", "fantastic", "love", "amazing", "wonderful"};
    std::vector<std::string> negativeWords = {"sad", "bad", "terrible", "hate", "awful", "angry", "depressed"};
    std::vector<std::string> neutralWords = {"Can't say", "don't know", "maybe","okay"};

    std::string lowerInput = toLowerCase(input);
    int score = 0;

    // Count occurrences of positive words
    for (const std::string &word : positiveWords)
    {
        size_t pos = lowerInput.find(word);
        while (pos != std::string::npos)
        {
            score++;
            pos = lowerInput.find(word, pos + word.length());
        }
    }

    // Count occurrences of negative words
    for (const std::string &word : negativeWords)
    {
        size_t pos = lowerInput.find(word);
        while (pos != std::string::npos)
        {
            score--;
            pos = lowerInput.find(word, pos + word.length());
        }
    }
    for (const std::string &word : neutralWords){
         size_t pos = lowerInput.find(word);
        while (pos != std::string::npos)
        {
            score=0;
            pos = lowerInput.find(word, pos + word.length());
        }
    }

    // Determine sentiment based on score
    std::string sentiment;
    if (score > 0)
    {
        sentiment = "positive";
    }
    else if (score < 0)
    {
        sentiment = "negative";
    }
    else
    {
        sentiment = "neutral";
    }

    // Return both score and sentiment as a pair
    return {score, sentiment};
}

// Complex question handling with a map of predefined answers
std::string handleComplexQuestions(const std::string &input)
{
    std::map<std::string, std::string> questionAnswers = {
        {"meaning of life", "The meaning of life is subjective. Many believe it's about finding happiness, purpose, and love."},
        {"who won the world cup in 2018", "France won the 2018 FIFA World Cup."},
        {"capital of france", "The capital of France is Paris."},
        {"who invented the lightbulb", "Thomas Edison is credited with inventing the lightbulb."}};

    std::string lowerInput = toLowerCase(input);

    for (const auto &pair : questionAnswers)
    {
        if (lowerInput.find(pair.first) != std::string::npos)
        {
            return pair.second;
        }
    }

    return "I'm not sure about that. Can you ask something else?";
}

// Function to check if a string contains a mathematical operation
bool containsMathOperation(const std::string &input)
{
    return input.find("add") != std::string::npos ||
           input.find("subtract") != std::string::npos ||
           input.find("multiply") != std::string::npos ||
           input.find("divide") != std::string::npos ||
           input.find('+') != std::string::npos ||
           input.find('-') != std::string::npos ||
           input.find('*') != std::string::npos ||
           input.find('/') != std::string::npos;
}

// Function to extract numbers and perform mathematical operations
std::string performMathOperation(const std::string &input)
{
    double num1, num2;
    char operation;

    // Using stringstream to extract numbers and operation from the input
    std::stringstream ss(input);
    ss >> num1;

    if (input.find("add") != std::string::npos || input.find('+') != std::string::npos)
    {
        operation = '+';
    }
    else if (input.find("subtract") != std::string::npos || input.find('-') != std::string::npos)
    {
        operation = '-';
    }
    else if (input.find("multiply") != std::string::npos || input.find('*') != std::string::npos)
    {
        operation = '*';
    }
    else if (input.find("divide") != std::string::npos || input.find('/') != std::string::npos)
    {
        operation = '/';
    }
    else
    {
        return "I couldn't detect a valid operation.";
    }

    ss >> num2;

    // Perform the corresponding operation
    double result;
    switch (operation)
    {
    case '+':
        result = num1 + num2;
        break;
    case '-':
        result = num1 - num2;
        break;
    case '*':
        result = num1 * num2;
        break;
    case '/':
        if (num2 == 0)
        {
            return "Division by zero is not allowed.";
        }
        result = num1 / num2;
        break;
    default:
        return "Operation not recognized.";
    }

    // Return the result as a string
    return "The result is: " + std::to_string(result);
}
void chatbot()
{
    ChatbotMemory memory;
    std::string userInput;

    std::vector<std::string> greetings = {"Hello! How can I assist you today?", "Hi! What's on your mind?", "Hey there! How are you?"};
    std::vector<std::string> farewellMessages = {"Goodbye! Have a wonderful day!", "Take care! See you next time!", "Bye! Stay safe!"};
    std::cout<<"Hello I am a chatBot What can I do for you?"<<std::endl;

    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin, userInput);

        if (toLowerCase(userInput) == "bye")
        {
            std::cout << "bye bye!!" << std::endl;
            break;
        }

        // Detect and respond to greetings
        if (userInput.find("hello") != std::string::npos || userInput.find("hi") != std::string::npos)
        {
            std::cout << "Hello again! How are you today?" << std::endl;
            continue;
        }

        // Sentiment analysis based on any positive or negative word
        std::pair<int, std::string> sentimentResult = analyzeSentiment(userInput);
        int sentimentScore = sentimentResult.first;
        std::string sentiment = sentimentResult.second;

        if (sentimentScore != 0)
        { // Trigger sentiment analysis if sentiment words are detected
            std::cout << "It sounds like you're feeling " << sentiment
                      << " today ." << std::endl;
            memory.lastSentiment = sentiment;
            continue;
        }

        // Answering complex questions
        std::string complexAnswer = handleComplexQuestions(userInput);
        if (complexAnswer != "I'm not sure about that. Can you ask something else?")
        {
            std::cout << complexAnswer << std::endl;
            memory.lastTopic = userInput;
            continue;
        }

        // Perform mathematical operations if detected
        if (containsMathOperation(userInput))
        {
            std::string mathResult = performMathOperation(userInput);
            std::cout << mathResult << std::endl;
            continue;
        }

        // Default fallback response
        std::cout << "I see. Can you tell me more about that?" << std::endl;
    }
}

int main()
{
    chatbot();
    return 0;
}
