#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
using namespace std;

// Change below values to adjust characteristics of system. All units are in SI.
const double h = 200;
const double L = 900;
const double R = 0.8;
const double b = 0.2;
const double rho = 1000;
const double mu = 1.307E-3;
const double g = 9.81;
const double Q_max = 0.3 * 27.1;
double d, d_n, alpha, Q, z_f, z_p1, V_p, h_fp, Re_p, Re_n, f, L_p, V_n, D, P, Omega, U, P_max, Omega_max, Beta, Efficiency; // variable parameters
int mode; // problem specific selector corresponding to defined problems in project manual.
string fileName; // csv file to work with

void setup();
void calculation();

void setup()
{
    if (mode == 3)
    {
        fileName = "prob3_calcs.csv";
        std::ofstream write(fileName);
        write.close();
    }
    else if (mode == 4)
    {
        fileName = "prob4_calcs.csv";
        std::ofstream write(fileName);
        write.close();
    }
    else if ( mode == 5)
    {
        fileName = "prob5_calcs.csv";
        std::ofstream write(fileName);
        write.close();
    }
    else if ( mode == 6)
    {
        fileName = "prob6_calcs.csv";
        std::ofstream write(fileName);
        write.close();
    }
    else
    {
        cout << "Invalid entry. Enter an integer from 3 - 6: ";
        cin >> mode;
        setup();
    }
}

void calculation()
{
    std::ofstream write;
    write.open(fileName);

    if (mode == 3) // Note that this calculation is nozzle limiting rather than Q limiting.
    {
        write << "alpha,h_f,V_n,Q,Re_p,Re_n\n";

        d = 0.5; // penstock diameter
        d_n = 0.08; // nozzle diameter
        z_f = h; // height of forbay level relative to datum
        
        // Things in for loop depend on alpha.
        for (int i = 5; i <= 10; i++)
        {
            alpha = i * M_PI / 180; // penstock angle (converted to radians)
            z_p1 = L * tan(alpha); // penstock hight relative to datum
            V_p = sqrt(2 * g * (z_f - z_p1)); // velocity in penstock
            Re_p = (rho * V_p * d) / mu; // Reynold's number in penstock
            f = pow(1.8 * log10(Re_p / 6.9), -2); // friction factor
            L_p = L / cos(alpha); // characteristic length of penstock
            h_fp = f * (L_p / d) * (V_p / (2 * g)); // friction head in penstock
            V_n = sqrt(2 * g * (z_f - h_fp)); // velocity at nozzle
            Re_n = (rho * V_n * d_n) / mu; // Reynold's number at nozzle
            Q = V_n * M_PI * pow(d_n / 2, 2); // flow rate
            write << to_string(i) << "," << to_string(h_fp) << "," << to_string(V_n) << "," << to_string(Q) << "," 
                << to_string(Re_p) << "," << to_string(Re_n) << endl; // add data to csv file
        }
    }
    /*Choosing to limit flow rate from here on out rather then limiting nozzle diamater.*/
    Q = Q_max;
    if (mode == 4) // Note that a frequency of 300 rpm indicates an addition of power to the system and is not possible in reality (discuss in report).
    {
        write << "d,P,d_n\n";

        z_f = h; // height of forbay level relative to datum
        D = 2 * M_PI * R; //distance travelled by wheel per one revolution
        Omega = 300 / 60; // frequency of turbine, converted to Hz or s^-1
        alpha = 10 * M_PI / 180; // penstock angle (converted to radians)
        z_p1 = L * tan(alpha); // penstock hight relative to datum
        V_p = sqrt(2 * g * (z_f - z_p1)); // velocity in penstock

        // Things in loop dependent on d.
        for (double i = 0.2; i <= 1.6; i += 0.1)
        {
            d = i;
            Re_p = (rho * V_p * d) / mu; // Reynold's number in penstock
            f = pow(1.8 * log10(Re_p / 6.9), -2); // friction factor
            L_p = L / cos(alpha); // characteristic length of penstock
            h_fp = f * (L_p / d) * (V_p / (2 * g)); // friction head in penstock
            V_n = sqrt(2 * g * (z_f - h_fp)); // velocity at nozzle
            d_n = (2 * Q) / (M_PI * V_n); // Nozzle diameter to achieve max Q
            P = 2 * rho * Q * V_n * D * Omega / 1E6; // power (Mega Watts)
            write << to_string(d) << "," << to_string(P) << "," << to_string(d_n) << endl; // add data to csv file
        }
    }
    else if (mode == 5)
    {
        write << "d,U,Omega_max,P_max,d_n\n";

        z_f = h; // height of forbay level relative to datum
        D = 2 * M_PI * R; //distance travelled by wheel per one revolution
        alpha = 10 * M_PI / 180; // penstock angle (converted to radians)
        z_p1 = L * tan(alpha); // penstock hight relative to datum
        V_p = sqrt(2 * g * (z_f - z_p1)); // velocity in penstock

        // Things in loop dependent on d.
        for (double i = 0.2; i <= 1.6; i += 0.1)
        {
            d = i;
            Re_p = (rho * V_p * d) / mu; // Reynold's number in penstock
            f = pow(1.8 * log10(Re_p / 6.9), -2); // friction factor
            L_p = L / cos(alpha); // characteristic length of penstock
            h_fp = f * (L_p / d) * (V_p / (2 * g)); // friction head in penstock
            V_n = sqrt(2 * g * (z_f - h_fp)); // velocity at nozzle
            Omega = (g * h) / (2 * D * V_n); // frequency of turbine Hz or s^-1
            Omega_max = Omega * 60; // max frequency in rpm
            U = R * Omega * 2 * M_PI; // tangential velocity for max power generation m/s
            d_n = (2 * Q) / (M_PI * V_n); // Nozzle diameter to achieve max Q
            P_max = 2 * rho * Q * V_n * D * Omega / 1E6; // power (Mega Watts)
            write << to_string(d) << "," << to_string(U) << "," << to_string(Omega_max) << "," << to_string(P_max) 
            << "," << to_string(d_n) << endl; // add data to csv file
        }

        /* Using data above, we can generate data for power dependent on turbine rpm */
        write << "\nd,Omega,P\n";

        for (int i = 0; i < 3; i++)
        {
            switch (i)
            {
            case 0:
                d = 0.2;
                for(double s = 224.1; s >= 60; s -= 10) // start at optimal rpm for 100% efficiency and test effects on power when reduced
                {
                    Re_p = (rho * V_p * d) / mu; // Reynold's number in penstock
                    f = pow(1.8 * log10(Re_p / 6.9), -2); // friction factor
                    L_p = L / cos(alpha); // characteristic length of penstock
                    h_fp = f * (L_p / d) * (V_p / (2 * g)); // friction head in penstock
                    V_n = sqrt(2 * g * (z_f - h_fp)); // velocity at nozzle
                    Omega = s / 60; // convert to hz
                    P = 2 * rho * Q * V_n * D * Omega / 1E6; // Mega Watts
                    write << to_string(d) << "," << to_string(s) << "," << to_string(P) << endl; // add data to csv file
                }
                break;
            case 1:
                d = 1;
                for(double s = 191.6; s >= 60; s -= 10) // start at optimal rpm for 100% efficiency and test effects on power when reduced
                {
                    Re_p = (rho * V_p * d) / mu; // Reynold's number in penstock
                    f = pow(1.8 * log10(Re_p / 6.9), -2); // friction factor
                    L_p = L / cos(alpha); // characteristic length of penstock
                    h_fp = f * (L_p / d) * (V_p / (2 * g)); // friction head in penstock
                    V_n = sqrt(2 * g * (z_f - h_fp)); // velocity at nozzle
                    Omega = s / 60; // convert to hz
                    P = 2 * rho * Q * V_n * D * Omega / 1E6; // Mega Watts
                    write << to_string(d) << "," << to_string(s) << "," << to_string(P) << endl; // add data to csv file
                }
                break;
            case 2:
                d = 1.5;
                for(double s = 189.9; s >= 60; s -= 10) // start at optimal rpm for 100% efficiency and test effects on power when reduced
                {
                    Re_p = (rho * V_p * d) / mu; // Reynold's number in penstock
                    f = pow(1.8 * log10(Re_p / 6.9), -2); // friction factor
                    L_p = L / cos(alpha); // characteristic length of penstock
                    h_fp = f * (L_p / d) * (V_p / (2 * g)); // friction head in penstock
                    V_n = sqrt(2 * g * (z_f - h_fp)); // velocity at nozzle
                    Omega = s / 60; // convert to hz
                    P = 2 * rho * Q * V_n * D * Omega / 1E6; // Mega Watts
                    write << to_string(d) << "," << to_string(s) << "," << to_string(P) << endl; // add data to csv file
                }
                break;
            }
        }
    }
    else if (mode == 6) // Note: calculations are flow limiting and Omega is optimized rather than using provied 300 rpm
    {
        write << "Beta,P,Efficiency\n";

        d = 1;
        z_f = h; // height of forbay level relative to datum
        D = 2 * M_PI * R; //distance travelled by wheel per one revolution
        alpha = 10 * M_PI / 180; // penstock angle (converted to radians)
        z_p1 = L * tan(alpha); // penstock hight relative to datum
        V_p = sqrt(2 * g * (z_f - z_p1)); // velocity in penstock
        Re_p = (rho * V_p * d) / mu; // Reynold's number in penstock
        f = pow(1.8 * log10(Re_p / 6.9), -2); // friction factor
        L_p = L / cos(alpha); // characteristic length of penstock
        h_fp = f * (L_p / d) * (V_p / (2 * g)); // friction head in penstock
        V_n = sqrt(2 * g * (z_f - h_fp)); // velocity at nozzle
        d_n = (2 * Q) / (M_PI * V_n); // Nozzle diameter to achieve max Q
        Omega = (g * h) / (2 * D * V_n); // frequency of turbine Hz or s^-1

        // Things in loop dependent on Beta.
        for (double i = 0; i <= 180; i += 1)
        {
            Beta = i;
            P = (V_n - (V_n * cos(Beta * M_PI / 180))) * rho * Q * D * Omega / 1E6; // power (Mega Watts)
            Efficiency = (P / 15.95106) * 100;
            write << to_string(Beta) << "," << to_string(P) << "," << to_string(Efficiency) << endl; // add data to csv file
        }
    }

    write.close();
}

int main()
{
    cout << "Enter calculation mode (3 - 6): ";
    cin >> mode;
    setup();
    calculation();

    cout << "Calculation successful! Data output to: " << fileName;
    return 0;
}