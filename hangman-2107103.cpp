#include <bits/stdc++.h>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <fstream>

using namespace std;


class parentPlayer
{
protected:
    string playerName;
    int playerScore;
public:
    virtual void setPlayerName(string name)=0;
    virtual void setPlayerScore(int a)=0;
    virtual string getPlayerName()=0;
    friend void checkInput(char inputByPlayer,int *wrongTurn,string &secretWord,string &userWord,string &wordClue,int ifMatch,parentPlayer *ob1,int *remainingLife,set<char>& mySet,long long int *timeGone);
};

class playerClass:public parentPlayer
{
protected:
public:
    playerClass()
    {
        playerName="Default User";
        playerScore=100;
    }
    void setPlayerName(string name)
    {
        playerName=name;
        playerScore=100;
    }
    void setPlayerScore(int a)
    {
        playerScore=a;
    }
    string getPlayerName()
    {
        return playerName;
    }
    friend void checkInput(char inputByPlayer,int *wrongTurn,string &secretWord,string &userWord,string &wordClue,int ifMatch,parentPlayer *ob1,int *remainingLife,set<char>& mySet,long long int *timeGone);
};

class secretWordClass
{
    string secretWordList[26]=
    {
        "APPLE", "BANANA", "CAT", "DOG", "ELEPHANT", "FLOWER", "GUITAR", "HONEY", "ISLAND", "JACKET",
        "KITE", "LEMON", "MOON", "NEST", "ORANGE", "PEAR", "QUEEN", "ROSE", "STAR", "TREE",
        "UMBRELLA", "VASE", "WATERMELON", "XYLOPHONE", "YACHT", "ZEBRA"
    };
    string wordClueList[26]=
    {
        "A common fruit.", "A popular tropical fruit.", "A small domesticated carnivorous mammal.",
        "A domesticated mammal known for loyalty.", "A large mammal with a trunk and tusks.",
        "A colorful and fragrant plant part.", "A musical instrument with strings.",
        "A sweet, sticky substance made by bees.", "A piece of land surrounded by water.",
        "A type of clothing worn on the upper body.", "A flying toy that dances in the wind.",
        "A sour yellow citrus fruit.", "A natural satellite that orbits the Earth.",
        "A bird's home made from twigs and leaves.", "A round, citrus fruit with a tough outer layer.",
        "A sweet and juicy fruit.", "A female ruler or monarch.",
        "A type of flower known for its beauty and fragrance.", "A luminous celestial object in the night sky.",
        "A tall woody plant with branches and leaves.", "A protective device used in the rain.",
        "A container often used for holding flowers.", "A large juicy fruit with green skin and red flesh.",
        "A musical instrument with wooden bars.", "A luxurious boat often used for sailing.",
        "A striped African animal known for its distinctive pattern.",
    };
    string generatedWord;
public:
    string generateSecrectWord(int *remainingLifebar)
    {

        int max;
        max = 25;
        srand(time(0));
        *remainingLifebar=rand()%max;

        generatedWord=secretWordList[(*remainingLifebar)];
        return generatedWord;
    }
    string generateWordClue(int remainingLifebar)
    {
        return wordClueList[remainingLifebar];
    }
    string generateUserWord()
    {
        string userWord(generatedWord.size(), '_');
        return userWord;
    }
};

void checkInput(char inputByPlayer,int *wrongTurn,string &secretWord,string &userWord,string &wordClue,int ifMatch,parentPlayer *ob1,int *remainingLife,set<char>& mySet,long long int *timeGone)
{
    ifMatch=0;
    if(inputByPlayer >= 'a' && inputByPlayer<='z')
    {
        inputByPlayer-=32;
    }

    string givenPunishment[7]= {"===========\n||     |\n||     |\n||    \n||    \n||    \n||\n=============","===========\n||     |\n||     |\n||     0\n||    \n||    \n||\n=============","===========\n||     |\n||     |\n||     0\n||    /\n||    \n||\n=============","===========\n||     |\n||     |\n||     0\n||    / \\\n||    \n||\n=============","===========\n||     |\n||     |\n||     0\n||    /|\\\n||    \n||\n=============","===========\n||     |\n||     |\n||     0\n||    /|\\\n||    /\n||\n=============","===========\n||     |\n||     |\n||     0\n||    /|\\\n||    / \\\n||\n============="};

    for(int i=0; i<secretWord.size(); i++)
    {
        if(inputByPlayer==secretWord[i])
        {
            userWord[i]=inputByPlayer;
            ifMatch=1;
        }
    }
    if(ifMatch==0)
    {
        int ltrChk=0;
        for (const char& usedLetter : mySet)
        {
            if(usedLetter==inputByPlayer)
            {
                ltrChk=1;
                break;
            }
        }
        if(ltrChk==0)
        {
            ob1->playerScore-=ceil(100/6);
            if(ob1->playerScore==4)
            {
                ob1->playerScore=0;
            }
            (*wrongTurn)++;
        }
    }

    cout<<endl<<givenPunishment[*wrongTurn]<<endl<<"Clue: "<<wordClue<<endl<<"Timeline: "<<userWord<<endl<<"Used Letters: ";

    mySet.insert(inputByPlayer);

    for (const char& usedLetter : mySet)
    {
        std::cout << usedLetter << " ";
    }

    cout<<"\nCurrent Score: "<<ob1->playerScore<<endl<<"Time remaining: "<<120-*timeGone<<endl<<endl;

    if (userWord == secretWord)
    {
        *remainingLife=0;
        cout<<"Congo, "<<ob1->playerName<<"! Your Score is: "<<ob1->playerScore<<endl;

        ofstream fout;
        fout.open("scores.txt", std::ios_base::app);
        fout << ob1->playerName <<endl<<ob1->playerScore<<endl;
        fout.close();

    }
    else if(*wrongTurn==6)
    {
        *remainingLife=0;
        cout<<"You are dead, "<<ob1->playerName<<"! The word was: "<<secretWord<<endl;
    }
    else if(*timeGone>=120)
    {
        cout<<"Time is over, "<<ob1->playerName<<"! The word was: "<<secretWord<<endl;
    }

}

void firstShow(string userWord,string wordClue)
{
    cout<<"\n\n===========\n||     |\n||     |\n||    \n||    \n||    \n||\n=============\n"<<"Clue: "<<wordClue<<endl<<"Timeline: "<<userWord<<endl<<"Time stars now... You have only 120seconds."<<endl<<endl;
}

void showHighScore()
{
    ifstream fin;
    fin.open("scores.txt");
    string hsName,tempname;
    int highScore=0,tempscore;
    while(fin.eof()==0)
    {
        fin>>tempname>>tempscore;
        if(tempscore>=highScore)
        {
            highScore=tempscore;
            hsName=tempname;
        }
    }
    if(highScore>0)
    {
        cout<<"\nLatest Highest Scorer: "<<hsName<<endl<<"Highest Score: "<<highScore<<endl<<endl;
    }
    fin.close();
}

int main()
{
    int menuchoice;

mainMenuSection:
    cout<<"\n\n\t###################\n\t######HANGMAN######\n\t######2107103######\n"<<endl;
    cout<<"1. Start New Game \n2. Check High Score \n3. Exit the game\n\nEnter your choice: ";
tryAgain:
    cin>>menuchoice;
    if(menuchoice==1)
    {
        set<char> usedChars;
startPlaying:
        cout<<"\n\nEnter Player Name: ";
        string name;
        cin>>name;
playSameName:
        playerClass pl1;
        pl1.setPlayerName(name);
        parentPlayer *p1 = &pl1;
        int wrongTurns=0,ifMatch=0,remainingLife=5,randomremainingLife;
        string secretWordGame, wordClueGame, userWordGame;
        secretWordClass swc1;
        secretWordGame=swc1.generateSecrectWord(&randomremainingLife);
        wordClueGame=swc1.generateWordClue(randomremainingLife);
        userWordGame=swc1.generateUserWord();
        firstShow(userWordGame,wordClueGame);


        char userInput;



        while(remainingLife!=0)
        {


            int countDownSeconds = 120;
            long long int zero=0;
            long long int *timeGone=&zero;

            auto startTime = chrono::high_resolution_clock::now();
            bool stopTimer=false;

            while (true)
            {

                if (*timeGone >= countDownSeconds  && remainingLife!=0)
                {
                    goto playAgainSection;
                    countDownSeconds=0;
                    stopTimer=true;
                }
                else if(wrongTurns==6 || remainingLife==0)
                {
                    countDownSeconds=0;
                    stopTimer=true;

                    break;
                }


                cout<<"Enter a letter: ";
                cin>>userInput;
                auto elapsedTime = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - startTime).count();

                timeGone=&elapsedTime;

                checkInput(userInput,&wrongTurns,secretWordGame,userWordGame,wordClueGame,ifMatch,p1,&remainingLife,usedChars,timeGone);
                this_thread::sleep_for(chrono::seconds(1));

            }
            usedChars.clear();
playAgainSection:
            char playAgain;
            cout<<"\n\nWant to play again?(y/n): ";
            cin>>playAgain;
            if(playAgain=='y'||playAgain=='Y')
            {
                char playerChange;
                cout<<"\nChange Player?(y/n): ";
                cin>>playerChange;
                if(playerChange=='y'|| playerChange=='Y')
                {
                    goto startPlaying;
                }
                else
                {
                    goto playSameName;
                }
            }
            else
            {
                goto mainMenuSection;
            }
        }
    }
    else if(menuchoice==2)
    {
        showHighScore();
        goto mainMenuSection;
    }
    else if(menuchoice==3)
    {
        cout<<"\n\nSee you again, Smart kiddo!"<<endl;
    }
    else
    {
        cout<<"\nWrong input. Enter again: ";
        goto tryAgain;
    }
    return 0;


}

