#include "ZMPT101B.h" /* Library untuk sensor ZMPT101B */
#include "ACS712.h" /* Library untuk sensor ACS712 */
#include "Wire.h" /* Library untuk lcd 16x2 I2C */
#include "LiquidCrystal_I2C.h" /* Library untuk lcd 16x2 I2C */

int sensor_tegangan_pin = A3;
int sensor_arus_pin = A2;

/* Inisialisasi sensor tegangan ZMPT101B */
ZMPT101B sensorTegangan(sensor_tegangan_pin);
/* Inisialisasi sensor arus ACS712 20A */
ACS712 sensorArus(ACS712_20A, sensor_arus_pin);
/* Inisialisasi lcd 16x2 I2C */
LiquidCrystal_I2C lcd(0x27);

void setup() {
    /* Setup serial monitor */
    Serial.begin(9600);
    /* Setup lcd */
    lcd.begin(16,2);

    /* Tampilkan Text kalibrasi */
    Serial.print("Memulai proses kalibrasi");
    lcd.print("Kalibrasi");
    for (int i=0; i <= 5; i++) {
        lcd.print('.');
        Serial.print('.');
        delay(500);
    }

    /* Mulai kalibrasi sensor tegangan */
    /* sensorTegangan.calibrate(); */
    /* Mulai kalibrasi sensor arus */
    sensorArus.calibrate();
    Serial.println("Selesai!");
}

void loop() {
    /* Hitung nilai tegangan dan arus */
    float Tegangan = sensorTegangan.getVoltageAC();
    float Arus = sensorArus.getCurrentAC();

    /*
     * Jika tagangan dibawah 20V maka dianggap 0V,
     * dikarenakan noise pada sensor ZMPT101B
     */
    if (Tegangan <= 20) Tegangan = 0.0;
    /*
     * Jika nilai arus dibawah 0.05A maka arus 0A,
     * dikarenakan resolusi Analog input Arduino Uno hanya 10bi(1024),
     * maka nilai arus dibawah 0.05A dianggap noise.
     * dapat disesuaikan 0.05, 0.07, 0.08
    */
    if (Arus <= 0.05) Arus = 0.0;

    /* Kalkulasi Daya */
    float Daya = Tegangan * Arus;

    lcd.clear();
    lcd.print("U:");
    lcd.print(Tegangan, 0);
    lcd.print("V");
    lcd.setCursor(8,0);
    lcd.print("I:");
    lcd.print(Arus, 2);
    lcd.print("A");
    lcd.setCursor(0,1);
    lcd.print("DAYA: ");
    lcd.print(Daya, 2);
    lcd.print("W");

    /* Jeda saat melakukan pengukuran tegangan dan arus
     * dapat berfariasi bergantung pada lokasi
     * 200, 300, 500, 1000
    */
    delay(200);
}
