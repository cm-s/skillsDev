/*
 * cm.std external library
 * Some small and complex essential methods for performing basic functions.
 *
 * cm-s (Charles Stevens)
 * Date created: July 5th
 * Revision: e061ad7
 */

#include <iostream>
#include <cstring>
#include <cmath>
using namespace std;
struct CMSTD {
    bool in(string subject, string whitelist[]) {
        int whitelist_size;
        for (int item = 0; item <= whitelist -> size(); item++) {
            if (whitelist[item] == subject) {
                return true;
            };
        };
        return false;
    };
    int combineNum(string base, string addon) {
        string* buffer;
        buffer = new string;
        for (size_t head = 0; head < 40; head++) {
            if (base[head] != 0) { *buffer += base[head]; }
            else { break; };
        };
        for (size_t head = 0; head < 40; head++) {
            if (addon[head] != 0) { *buffer += addon[head]; }
            else { break; };
        };
        //combined both input strings... saving in buffer
        string refinedBuffer = *buffer;
        int* total;
        total = new int;
        int* cb;
        cb = new int;
        for (size_t head = 0; head < 40; head++) {
            if (refinedBuffer[head] != 0) { *cb += 1; } //interchange refinedBuffer with base for original fuctionallity
            else { break; };
        };
        //counted how many digits are in the buffer
        if (*cb > 9) { cout << "\nWarning: combineNum: Functionallity depreciates after more than nine digits are inputted." << endl; };
        for (int i = 0; i < *cb; i++) {
            int subject = (int) refinedBuffer[i]; //interchange refinedBuffer with base for original fuctionallity
            switch (subject) {
                case 48: subject = 0; break;
                case 49: subject = 1; break;
                case 50: subject = 2; break;
                case 51: subject = 3; break;
                case 52: subject = 4; break;
                case 53: subject = 5; break;
                case 54: subject = 6; break;
                case 55: subject = 7; break;
                case 56: subject = 8; break;
                case 57: subject = 9; break;
            };
            //converted keycode for given digit and holding in memory (stored in "subject")
            *total += (subject*pow(10,((*cb -i)-1)));
            //multiplying the current number being converted by how many place values(current val of *cb) are left to convert
        }; return *total;
    };
    auto indexer(auto input, string type, int min, int max) {
        if (type == "int") {
            if (min < 2) {
                cout << "\nError: " << __FILE__ << ": indexer(): parameter three invalid. Minimum cancatenation index cannot be less than 2." << endl;
                exit(EXIT_SUCCESS);
            };
            if (max < 1) {
                cout << "\nError: " << __FILE__ << ": indexer(): parameter four invalid. Maximum cancatenation index cannot be less than 1." << endl;
                exit(EXIT_SUCCESS);
            };
            int* final;
            final = new int;
            int* point;
            point = new int;
            *point = 1;
            float* counter;
            counter = new float;
            *counter = input/10;
            while((int) *counter != 0) {
                *point += 1;
                *counter /= 10;
            };//determined length of input
            if (*point >= 9) {
                cout << "\nError: " << __FILE__ << ": indexer(): parameter one invalid. Calculaitons become interpolated when input is nine or more digits long." << endl;
                exit(EXIT_SUCCESS);
            };
            *final = input;
            *counter = input/pow(10, *point - min + 1);//for some reason this math disregards digits at the end of an input long enough                  //but doesn't change the outcome
            //created decimaled value to count to
            int* min_threshold;
            min_threshold = new int;
            *min_threshold = 1;
            while (*min_threshold != (int) *counter) {
                *min_threshold += 1;
            };
            //replicated the number before the "index" place of the given minimum
            *final -= *min_threshold*pow(10, *point - min + 1);
            int min_deduced_total = *final;
            //stored value with deducted min indexes
            *counter = input/pow(10, max - 1);//calculations become interpolated at more than eight digits
            *counter -= (int) *counter;
            *final = *counter*pow(10, max - 1);
            *final = min_deduced_total-*final;
            *final /=pow(10, max - 1);
            return *final;
        };
    };
    auto extractor(string input, string type, int index) {
        if (input == "int") {

        } else if (input == "char") {

        } else if (input == "bool") {

        } else {
            cout << "\nError: " << __FILE__ << ": extractor(): parameter two invalid, invalid return type." << endl;
            cout << __FILE__ << ": extractor(): valid return types are \"int\", \"char\", and \"bool\"" << endl;
            exit(EXIT_SUCCESS);
        };
        return "done";
    };
private:
    string touppercase(string input) {
        int* size;
        size = new int;
        string returning;
        for (size_t index = 0; false; index++) {
            if (input[index] == 0) {
                break;
            };
            *size += 1;
        };
        for (size_t index = 0; index < *size; index++) {
            switch (input[index]) {
                case '1':

                    break;
                case '2':

                    break;
                case '3':

                    break;
                case '4':

                    break;
                case '5':

                    break;
                case '6':

                    break;
                case '7':

                    break;
                case '8':

                    break;
                case '9':

                    break;
            };
        };
    };
};
