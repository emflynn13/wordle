// Emma Flynn
#include <stdio.h>
#include "utils.h"
#include "time.h"
//#include <string.h>


const int dict_size = 5757;
char dictionary[dict_size][6];
float wins = 0;
int gamesPlayed = 0;
char wordArray[6];   // word randomly chosen from dictionary
int word_size = 5;
char correctLetters[5];
int correctLettersSize = 0;
char incorrectLetters[21];  // array of incorrect letters used for hard mode
int incorrectLength = 0;

char charsInEachGame[26];   // add each unique letter used in each game to this array - for stats
int charListLength = 0;     // track length of charInEachGame

int numLettersUsedPerGame[15];  // array containing the number of unique letters used per game - for stats
int sizeLettersPerGameArray = 0; // for stats



// implementation of strcmp. returns 0 if strings are the same
int stringCmp(char *userGuess, char *dictionaryIndex){
    while (*userGuess && (*userGuess == *dictionaryIndex)) {
        userGuess++;
        dictionaryIndex++;
    }
    return *userGuess - *dictionaryIndex;
}

char *chooseWord(){
    // get random number and retrieve word at that index in dictionary
    load_word_list(dictionary);
    //srand(time(NULL));
    int randNum = rand() % 5757;
    int i;
    // add word to wordArray
    for (i = 0; i < word_size; i++) {
        wordArray[i] = dictionary[randNum][i];
    }
    wordArray[i] = '\0';

    // for testing
//    for (i = 0; i < word_size; i++) {
//        printf("%c", wordArray[i]);
//    }
//    printf("\n");
}

// create a string containing the users guess and then validate it.
// userGuess array has a size of 10 to allow for user error when entering their guess rather than calculating and clearing
// a buffer using getchar(). Ensures length is exactly 5, nothing other than letters are accepted and the guess is a word
// present in the dictionary
int getUserInput(char *userGuess) {
    int i = 0;
    int j;
    int result = 1;
    char input = 0;
    int index;
    int numLetters;

    printf("Enter a word: ");
    while (input != '\n') {
        input = getchar();
        if (input != '\n') {
            if (input >= 'A' && input <= 'Z') {
                input += 32;   // convert to lower case
                userGuess[i] = input;
                i++;
            }else {
                userGuess[i] = input;
                    i++;
            }
        }
    }
    userGuess[i] = '\0';

    // ensure all entries are letters
    for (j = 0; j < i; j++)
        if (userGuess[j] < 'A' || userGuess[j] > 'z') {
            printf("Only letters please.\n");
            return 0;
        }

    // ensure user guess has the correct amount of characters
    numLetters = i - word_size;
    if (numLetters > 0 || numLetters < 0) {
        printf("Entries can only be 5 letters.\n");
        return 0;
    }

    // check if guess is in the dictionary of words, return 1 if it is, 0 if it isn't
    for (index = 0; (result != 0) && (index < 5757); index++) {
        result = stringCmp(userGuess, dictionary[index]);

        //result = strcmp(userGuess, dictionary[index]);
    }
    if (result != 0) {
        printf("Invalid entry, please try again.\n");
        for (j = 5; j > i; j--) // clear input buffer
            getchar();
        return 0;
    }
    else
        return 1;
}

int hardModeValidation(char *userGuess, char *correctLetterOrderArray){
    int i;
    int j;
    int found = 0;

    // if a character is present at a specific index if the correct letter array (meaning it's position
    // has already correctly been guessed) and the user does not
    // include the character at this index in their current guess it is deemed invalid
    for (i = 0; i < 5; i++){
        if (correctLetterOrderArray[i] != '_') {
            if (userGuess[i] != correctLetterOrderArray[i]) {
                printf("Must use letters in correct position.\n");
                return 0;
            }
        }
        for (j = 0; j < incorrectLength; j++)
            if (userGuess[i] == incorrectLetters[j]) {
                printf("Cannot use letters that have previously been deemed incorrect.\n");
                return 0;
            }
    }

    // compare characters from user's guess to previous correct letters guess to ensure they are used
    // in each remaining guess
    // for each character in the correctLetter array, compare it to each letter in the new guess
    for (i = 0; i < correctLettersSize; i++) {
        for (j = 0; j < 5; j++)
            if (correctLetters[i] == userGuess[j])
                found = 1;
        if (!found) {
            printf("Must include all previous correct letters.\n");
            return 0;
        }
        found = 0;  //set found back to 0 at the end of each iteration to make sure each letter
                    // is searched for
    }
    return 1;
}

// method for keeping track of characters used in each game for stats at the end
void createListCharsUsed(char *userGuess) {
    int i;
    int foundInCharList = 0;

    // iterate through array and if it's not already present then add it
    for (i = 0; i < 5; i++) {
        for (int j = 0; j < 26; j++){
        if (userGuess[i] == charsInEachGame[j])
            foundInCharList = 1;
        }
        if (!foundInCharList) {
            charsInEachGame[charListLength] = userGuess[i];
            charListLength++;
        }
        foundInCharList = 0;
    }


}

int matchGuess(char *userGuess, char *correctLetterOrderArray){
    int i;
    int m;
    int count = 0;
    int foundInWord = 0;
    int foundInIncorrect = 0;
    int isPresent = 0;

    for (i = 0; i < word_size; i++) {

        // if input char at specific index correctly matches the char at the same index in wordArray then print it
        if (userGuess[i] == wordArray[i]) {
            printf("%c", userGuess[i]);
            correctLetterOrderArray[i] = userGuess[i];
            isPresent = 1;
            count++;
        }
        else {
            printf("_");
            correctLetterOrderArray[i] = '_';
                // iterate through correct word to check if char at index matches any of the chars in wordArray
                for (m = 0; m < word_size; m++)
                    if (userGuess[i] == wordArray[m]) {
                        isPresent = 1; // character is present in 'wordArray'
                    // then iterate through 'correctLetter' array to make sure the char isn't already in it
                    // if it is, set found to 1 because we only want it to appear once
                    for (int index = 0; index < word_size; index++)
                        if (userGuess[i] == correctLetters[index])
                            foundInWord = 1;
                    // if not already present then all the char at specific index to correctLetters array
                    if (!foundInWord) {
                        correctLetters[correctLettersSize] = userGuess[i];
                        correctLettersSize += 1;
                    }
                }
        }
        if (!isPresent) { // if char is not present in wordArray
            // iterate though the incorrectLetters array to see if the char is already present there
            for (m = 0; m < incorrectLength; m++)
                if (userGuess[i] == incorrectLetters[m])
                    foundInIncorrect = 1;
            if (!foundInIncorrect) {
                // if not found in incorrectLetters already then add it
                // prevent repeating chars in this array
                incorrectLetters[incorrectLength] = userGuess[i];
                incorrectLength++;
            }
        }
        // reset for next letter
        isPresent = 0;
        foundInWord = 0;
        foundInIncorrect = 0;
    }

    if (count == 5) {
        printf("\nCongrats! You win!\n");
        wins++;
        return 1;
    }
    else {
        printf("\n");
        // print correct letters that were in incorrect position
        printf("correct letters: ");
        for (i = 0; i < correctLettersSize; i++)
            printf("%c", correctLetters[i]);

        return 0;
    }
}

float getPercentageAlphabetUsed(){
    int i;
    float sum = 0;

    // find total number of characters used in all games
    for (i = 0; i < sizeLettersPerGameArray; i++)
        sum += (float) numLettersUsedPerGame[i];

    // find average
    return (sum / (float)(gamesPlayed * 26)) * 100.0;
}

void getStats(int *numGuessesArray){
    int i, j;
    float avgPercentAlpha;
    float percentage = (wins / (float) gamesPlayed) * 100;
    printf("\n---------------\n");
    printf("Statistics Report\n");
    printf("You won %0.2f%% of games.\n", percentage);

    avgPercentAlpha = getPercentageAlphabetUsed();
    printf("Average Percentage of Alphabet: %0.2f%%\n", avgPercentAlpha);

    printf("\nGuess Histogram\n");
    for (i = 0; i < gamesPlayed; i++) {
        printf("Game %d: ", i + 1);
        for (j = 0; j < numGuessesArray[i]; j++) {
            printf("*");
        }
        printf("\n");
    }

    printf("\n---------------\n");

}


int main() {
    char userGuess[10];
    int true = 1;
    int remainingGuesses = 6;
    int validInput = 0;
    char mode;
    int hardModeValid = 0;
    int correct = 0;
    char correctLetterOrderArray[6];
    int numGuessesArray[25];
    int numGuessesLength = 0;
    int i;

    while (true) {
        printf("\nWould you like to enter hard mode?\nEnter 'y' for yes or any other key for no: ");
        scanf("%c", &mode);
        getchar();
        // load word from file
        chooseWord();
        while (remainingGuesses > 0){
            if (mode == 'y'){
                while (!hardModeValid) {
                    while (!validInput) {
                        validInput = getUserInput(userGuess);
                    }
                    validInput = 0; // reset to 0 for next round's guess
                    if (remainingGuesses < 6) // no letters to compare on first guess
                        hardModeValid = hardModeValidation(userGuess, correctLetterOrderArray);
                    else
                        hardModeValid = 1; // break out of loop for first guess because nothing to compare to
                }
                hardModeValid = 0;  // reset for next guess
                createListCharsUsed(userGuess);
                correct = matchGuess(userGuess,correctLetterOrderArray);

                if (!correct) {
                    printf("\nincorrect letters: ");
                    for (i = 0; i < incorrectLength; i++) {
                        printf("%c ", incorrectLetters[i]);
                    }
                }
            }
            else {
                while (!validInput) {
                    validInput = getUserInput(userGuess);
                }
                validInput = 0;
                createListCharsUsed(userGuess);
                correct = matchGuess(userGuess,correctLetterOrderArray);
            }
            if (correct)
                break;
            else {
                remainingGuesses--;
                printf("\n\nYou have %d remaining guesses.\n", remainingGuesses);
            }
        }
        if (remainingGuesses == 0){
            printf("Sorry you've lost.\nCorrect word was: ");
            for (i = 0; i < word_size; i++) {
                printf("%c", wordArray[i]);
            }
        }

        // for stats
        // append the number of letters used per game to the end of numLettersUsedPerGame array
        numLettersUsedPerGame[sizeLettersPerGameArray] = charListLength;
        sizeLettersPerGameArray++;

        // empty array after each game for new set of letters
        for (i = 0; i < charListLength; i++)
            charsInEachGame[i] = 0;
        charListLength = 0;

        printf("\n\n");
        printf("Would you like to continue? Enter 'y' for yes or any other character for no: ");
        char input = getchar();
        getchar();
        if (input != 'y') {
            printf("Bye\n");
            true = 0; // break while loop, end game
        }

        // append number of guesses it took to solve the game to the numGuessesArray for stats
        numGuessesArray[numGuessesLength] = 6 - (remainingGuesses - 1);  // -1 because correct guess needs to be subtracted
        numGuessesLength++;

        remainingGuesses = 6; // reset guesses for next game
        gamesPlayed++;


        // clear list after every game
        for (i = 0; i < correctLettersSize; i++){
            correctLetters[i] = 0;
        }

        correctLettersSize = 0; // reset after each game
        incorrectLength = 0; // reset after each game
    }

    getStats(numGuessesArray);
	return 0;
}