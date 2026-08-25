#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include "BP.h"
using namespace std;

int converter(string diagnosis){

    if (diagnosis == "Presence"){
       
        return 1;
    }
    else if (diagnosis == "Absence"){

        return 0;
    }
    else{

        return 0;
    }
}

int higher_value(double n1, double n2){
    if(n1 >= n2){

        return 0; // THIS IS FIRST;
    }
    else{
        return 1; // this is SECOND
    }
}

struct MConf{
    int c1 = 0;
    int c1_wrong_c0 = 0;
    int c0 = 0;
    int c0_wrong_c1 = 0;
};

int main()
{

    vector<vector<double>> people;
    fstream file;

    file.open("Heart_Disease_Prediction.csv", ios::in);

    if (file.is_open())
    {

        string line, word;

        getline(file, line); 

        while (getline(file, line))
        {

            stringstream s(line);

            string word;
            int data_number = 0;

            int real_number = 0;
            vector<double> person;
            
            for (int i = 0; i < line.length(); i++)
            {

                if (line[i] != ',')
                    word += line[i];

                else
                {
                    double data = stod(word) / 100;
                    
                    //cout << real_number << " " << data << "|" << endl;

                    person.push_back(data);

                    real_number++;
                    
                    data_number++;
                    word = "";
                }
            }
            
            
            word.pop_back(); // it has a \n at the end
            
            
            int converted_value = converter(word);

            if (converted_value == 0){

                person.push_back(1.0);
                person.push_back(0.0); // probability of ABSENCE
            }
            else if (converted_value == 1){
                person.push_back(0.0); // probability of PRESENCE
                person.push_back(1.0); 
            }
            else{
                cout << "no";
            }

            people.push_back(person);
        }
    }

    vector<vector<double>> input_people;
    vector<vector<double>> output_people;

    
    for(int i = 0; i < people.size(); i++){

        int num_param = 0;

        vector<double> pred;
        vector<double> norm;
        for(int j = 0; j < people[i].size(); j++){

            if (num_param == 13 || num_param == 14){
                // prediction
                pred.push_back(people[i][j]);
            }
            else{
                norm.push_back(people[i][j]);
            }

            //cout << people[i][j] << " ";

            num_param++;
        }
        //cout << endl;

        input_people.push_back(norm);
        output_people.push_back(pred);
    }

    vector<vector<double>> X_train, Y_train, X_test, Y_test;
    double train_percentage = 70;
    double test_percentage = 100.0 - train_percentage;
    
    double train_count = input_people.size() * (train_percentage / 100);
    double test_count = input_people.size() * (test_percentage / 100);

    cout << "training rows " << train_count << ", test rows " << test_count << ": " << train_count + test_count << endl;
    

/* 
    for(int i = 0; i < people.size(); i++){

        for(int j = 0; j < input_people[i].size(); j++){
            cout << input_people[i][j];
        }
        cout << "   " ;
        for(int j = 0; j < output_people[i].size(); j++){
            cout << output_people[i][j];
        }

        cout << endl ;

    }
*/

    for (int i = 0; i < train_count; i++){
        
        X_train.push_back(input_people[i]);
        Y_train.push_back(output_people[i]);
        //cout << i+1 << "," ;
    }

    for (int i = input_people.size() - test_count; i < input_people.size(); i++){
        
        X_test.push_back(input_people[i]);
        Y_test.push_back(output_people[i]);
        //cout << i+1 << "," ;
    }

    char activation = ' ';
    cout << "What type of activation do you want? (s: sigmoid, r:relu, t:tanh) ";
    cin >> activation;

    if(activation == 's'){
        cout << "USING SIGMOID ACTIVATION" << endl;
    }
    else if (activation == 'r'){
        cout << "USING RELU ACTIVATION" << endl;
    }
    else if (activation == 't'){
        cout << "USING TANH ACTIVATION" << endl;
    }
    else{
        cout << "NO ACTIVATION (LINEAR)" << endl;
    }

    BackPropagation heart_network({13, 10, 2});
    heart_network.learning_rate = 0.01;
    double mse = heart_network.train(X_train, Y_train, 300, activation);

    /*
    vector<vector<double>> prediction_heart_test;

    MConf m;

    cout << endl;
    for(int i = 0; i < test_count; i++){
        prediction_heart_test.push_back(heart_network.forward(X_test[i], activation));
        
        int higher = higher_value(prediction_heart_test[i][0], prediction_heart_test[i][1]);
        cout << "OBTAINED " << prediction_heart_test[i][0] << ", "<< prediction_heart_test[i][1] << " is category " << higher << "º " << " AGAINST " << Y_test[i][0] << ", "<<  Y_test[i][1] ;
        int real = higher_value(Y_test[i][0], Y_test[i][1]);

        if(higher == real){

            cout << " OK";

            if (higher == 0){
                m.c0++;
            }
            else if(higher == 1){
                m.c1++;
            }
            else{
                // cout << 
            }
        }
        else{
            cout << " NO";

            if(real == 0 && higher == 1){
                m.c0_wrong_c1++;
            }
            else if(real == 1 && higher == 0){
                m.c1_wrong_c0++;
            }
            else{
                // cout << 
            }
        }
        cout << endl;
        
    }

    double validation_mse = 0.0;
    int N = Y_test.size();

    for(int i = 0; i < N; i++){
        double sample_error = 0.0;
        for(int j = 0; j < 2; j++){
            double diff = Y_test[i][j] - prediction_heart_test[i][j];
            sample_error += diff * diff;
        }
        sample_error /= 2.0;  // there are 2 onehot classes
        validation_mse += sample_error;
    }

    validation_mse /= N; 

    double MAPE = 0.0;

    for(int i = 0; i < N; i++){

        int real_class = higher_value(Y_test[i][0], Y_test[i][1]); 
        double pred_value = prediction_heart_test[i][real_class];
        double ape = abs(1.0 - pred_value); 

        //cout << "Comparing mine " << pred_value << " with " << real_class << endl;
        
        MAPE += ape;
    }

    MAPE = (MAPE / N) * 100.0;

    int correct = 0;

    for(int i = 0; i < N; i++){
        int real_class = higher_value(Y_test[i][0], Y_test[i][1]);   // index of 1
        int pred_class = higher_value(prediction_heart_test[i][0], prediction_heart_test[i][1]); // index of max

        if(real_class == pred_class) correct++;
    }

    double accuracy = (double(correct) / N) * 100.0;

    int tp[2] = {0, 0};   
    int fn[2] = {0, 0}; 

    for(int i = 0; i < N; i++){
        int real_class = higher_value(Y_test[i][0], Y_test[i][1]);
        int pred_class = higher_value(prediction_heart_test[i][0], prediction_heart_test[i][1]);

        if(real_class == pred_class){
            tp[real_class]++;
        } else {
            fn[real_class]++;
        }
    }

    for(int c = 0; c < 2; c++){
        double recall = 0.0;
        if(tp[c] + fn[c] > 0){
            recall = (double)tp[c] / (tp[c] + fn[c]);
        }
        cout << "Recall class " << c << ": " << recall * 100 << "%" << endl;
    }

    cout << "MAPE: " << MAPE << " accuracy: " << accuracy << "%" << endl; 
    cout << "Training MSE: " << mse  << endl;
    cout << "Validation MSE: " << validation_mse << endl << endl;

    cout << "HEART DISEASE CONFUSION MATRIX" << endl;

    cout << "CLASS 0    " << m.c0 << "          " << m.c0_wrong_c1 << endl;
    cout << "CLASS 1    " << m.c1_wrong_c0 << "          " << m.c1  << endl;
   
    cout << "           CLASS 0    CLASS 1      (fictitious class)" << endl;

    cout << endl << "INDEX" << endl;
    cout << "CLASS 0: Absence" << endl <<  "CLASS 1: Presence" << endl;

    return 0;
    */
}