#define SSD1306_RST       RC1_bit      // SSD1306 OLED reset pin definition
#define SSD1306_RST_DIR   TRISC1_bit

#define LED       RC2_bit
#define LED_DIR   TRISC2_bit

#define  code1_on    1278825104
#define  code1_off   1278825088

#define  code2_on    1278825105
#define  code2_off   1278825089

#define  code3_on    1278825106
#define  code3_off   1278825090

#define  code4_on    1278825107
#define  code4_off   1278825091

#define  code5_on    1278825108
#define  code5_off   1278825092

#define  code6_on    1278825109
#define  code6_off   1278825093

#define  code7_on    1278825110
#define  code7_off   1278825094

#define  code8_on    1278825111
#define  code8_off   1278825095

#define  code9_on    1278825112
#define  code9_off   1278825096

#define  code10_on    1278825113
#define  code10_off   1278825097

#define  code11_on    1278825114
#define  code11_off   1278825098

#define  code12_on    1278825115
#define  code12_off   1278825099

#define  code13_on    1278825116
#define  code13_off   1278825100

#define  code14_on    1278825117
#define  code14_off   1278825101

#define  code15_on    1278825118
#define  code15_off   1278825102

#define  code16_on    1278825119
#define  code16_off   1278825103

//-------------------------------- Library -----------------------------------//
#include "SSD1306.c"
#include "GFX_Library.c"
#include "DIO.c"
//---------------------------- Variables Globales ----------------------------//
int choix_user;
unsigned long TIMER;
unsigned long Code_ON = 0;
unsigned long Code_OFF = 0;
//--------------------------- Declaration Prototype --------------------------//
void compteur_choix_user(void);
void Ecran_oled(void);
void Pop_up_ON(void);
void Pop_up_OFF(void);
void Incrementation(void);
void Decrementation(void);

void main ()
{
//------------------- Reglage de la fréquence d'horloge ----------------------//
 OSCCON = 0x66;
//----------------------------- Config Port B --------------------------------//
  ANSELB = 0;      // configure all PORTB pins as digital
  TRISB = 0b11111101;       // Configuration des entrée du port B
  

  INTCON.INT0IE = 1;           // peripherique interrupt

  INTCON2.RBPU = 0;  //configuration des pull up du port B
  WPUB = 0x3C;       //choix des pull up du port B

  C1ON_bit = 0;                    // Disable comparators
  C2ON_bit = 0;                    // Disable comparators
  
  INTCON.GIE = 1;            // Global interrupt
//----------------------------- Config Port C --------------------------------//
  ANSELC = 0;      //Configure all PORTC pins as digital
  LED_DIR = 0;
  
  LED=1;              //Allume la LED
  delay_ms (1000);    // Delai de 1s
  LED=0;              //Eteint la LED

  I2C1_Init(100000); // initialize I2C with clock frequency of 400kHz
//------------------------ Initialisation Variable ---------------------------//

  choix_user =1;  // initialise la valeur de choix_user � 1
  TIMER =0;  // initialise la valeur de choix_user � 1

//-------------------------- Initialisation OLED -----------------------------//
  SSD1306_begin(SSD1306_SWITCHCAPVCC, 0x78);
  // initialize with the I2C addr 0x7A (for the 128x64)

  display();        // Initialisation
  delay_ms (100); // Temps apres l'inistialisation

  display_clear(); // Supprimer tous ce qui est sur l'ecran
  display();// Actualise l'ecran
  display_setTextColor(WHITE,BLACK); // Couleur du Texte
  Ecran_oled();
//---------------------------- Bouton poussoir -------------------------------//
 while(1) 
 {
  if ((PORTB & 32 ) == 0x00){// Boutons vers le haut
  choix_user++; //Décrémentation des boutons pour la sélection
  Incrementation();
  Ecran_oled();
  TIMER=0;
  }
//----------------------------------------------------------------------------//
  if ((PORTB & 8 ) == 0x00){ // Bouton vers le bas
  choix_user--; //Décrémentation des boutons pour la sélection
  Decrementation();
  Ecran_oled();
  TIMER=0;
  }
//----------------------------------------------------------------------------//
  if ((PORTB & 4 ) == 0x00){ // Bouton code ON
  // Bouton ON
  LED=1;
  Dio_send(Code_ON);
  Pop_up_ON();
  delay_ms (500);
  Ecran_oled();
  LED=0;
  TIMER=0;
  }
//----------------------------------------------------------------------------//
  if ((PORTB & 16  ) == 0x00){ // Bouton code OFF
  // Bouton OFF
  LED=1;
  Dio_send(Code_OFF);
  Pop_up_OFF();
  delay_ms (500);
  Ecran_oled();
  LED=0;
  TIMER=0;
  }
  //----------------------------------------------------------------------------//
  if(TIMER >= 1000000){
  TIMER=0;
  ssd1306_command(SSD1306_DISPLAYOFF);
  asm sleep;
  ssd1306_command( SSD1306_DISPLAYON);
  }
  //----------------------------------------------------------------------------//
  delay_us(1);
  TIMER ++;
 }
}
 //----------------------------- Interface IHM -------------------------------//


void interupt() iv 0x0008 ics ICS_AUTO{
 if(INTCON.INT0IF==1)
 {
    INTCON.INT0IF=0;
 }
}

 void Incrementation(void){
 if (choix_user == 0){ //Laisse la variable � 1
  choix_user = 1;
  }
 }
 
 void Decrementation(void){
 if (choix_user == 17){ //Laisse la variable � 16
  choix_user = 16;
  }
 }
 
 void Pop_up_ON(void){
  display_fillRoundRect(32, 24, 64, 32,2, BLACK);
  display_drawRoundRect(32, 24, 64, 32,5, WHITE);
  display_setCursor(52, 28);
  display_setTextSize(1);
  display_printf("Code \r\n");
  display_setCursor(52,37 );
  display_setTextSize(2);
  display_printf("ON\r\n");
  display();
 }
 
 void Pop_up_OFF(void){
  display_fillRoundRect(32, 24, 64, 32,2, BLACK);
  display_drawRoundRect(32, 24, 64, 32,5, WHITE);
  display_setCursor(52, 28);
  display_setTextSize(1);
  display_printf("Code\r\n");
  display_setCursor(48, 37);
  display_setTextSize(2);
  display_printf("OFF\r\n");
  display();
 }

 void Ecran_oled(void){
              display_drawRoundRect(0, 0, 128, 64,2, WHITE);
              display_drawRoundRect(0, 0, 128, 16,2, WHITE);
              display_setCursor(16, 4);
              display_setTextSize(1);
              display_printf("Telecommande DIO\r\n");
              //Afficheur de base
   switch (choix_user){
    case 1 : {
              display_fillRoundRect(3, 20, 100, 40,2, BLACK);
              display_fillCircle(7, 24, 3, WHITE);
              display_drawCircle(7, 34, 3, WHITE);
              display_drawCircle(7, 44, 3, WHITE);
              display_drawCircle(7, 54, 3, WHITE);
              //Slider
              display_setCursor(14, 21);
              display_setTextSize(1);
              display_printf("Canal 1\r\n");

              display_setCursor(14, 31);
              display_setTextSize(1);
              display_printf("Canal 2\r\n");

              display_setCursor(14, 41);
              display_setTextSize(1);
              display_printf("Canal 3\r\n");

              display_setCursor(14, 51);
              display_setTextSize(1);
              display_printf("Canal 4\r\n");

             display();

             Code_ON = code1_on;
             Code_OFF = code1_off;
             break;
             };
    case 2 : {
              display_fillRoundRect(3, 20, 100, 40,2, BLACK);
              display_drawCircle(7, 24, 3, WHITE);
              display_fillCircle(7, 34, 3, WHITE);
              display_drawCircle(7, 44, 3, WHITE);
              display_drawCircle(7, 54, 3, WHITE);
              //Slider
              display_setCursor(14, 21);
              display_setTextSize(1);
              display_printf("Canal 1\r\n");

              display_setCursor(14, 31);
              display_setTextSize(1);
              display_printf("Canal 2\r\n");

              display_setCursor(14, 41);
              display_setTextSize(1);
              display_printf("Canal 3\r\n");

              display_setCursor(14, 51);
              display_setTextSize(1);
              display_printf("Canal 4\r\n");

              display();

               Code_ON = code2_on;
               Code_OFF = code2_off;
               break;
             };
    case 3 : {
              display_fillRoundRect(3, 20, 100, 40,2, BLACK);
              display_drawCircle(7, 24, 3, WHITE);
              display_drawCircle(7, 34, 3, WHITE);
              display_fillCircle(7, 44, 3, WHITE);
              display_drawCircle(7, 54, 3, WHITE);
              //Slider
              display_setCursor(14, 21);
              display_setTextSize(1);
              display_printf("Canal 1\r\n");

              display_setCursor(14, 31);
              display_setTextSize(1);
              display_printf("Canal 2\r\n");

              display_setCursor(14, 41);
              display_setTextSize(1);
              display_printf("Canal 3\r\n");

              display_setCursor(14, 51);
              display_setTextSize(1);
              display_printf("Canal 4\r\n");

               display();
               Code_ON = code3_on;
               Code_OFF = code3_off;
               break;
             };
    case 4 : {
              display_fillRoundRect(3, 20, 100, 40,2, BLACK);
              display_drawCircle(7, 24, 3, WHITE);
              display_drawCircle(7, 34, 3, WHITE);
              display_drawCircle(7, 44, 3, WHITE);
              display_fillCircle(7, 54, 3, WHITE);
              //Slider
              display_setCursor(14, 21);
              display_setTextSize(1);
              display_printf("Canal 1\r\n");

              display_setCursor(14, 31);
              display_setTextSize(1);
              display_printf("Canal 2\r\n");

              display_setCursor(14, 41);
              display_setTextSize(1);
              display_printf("Canal 3\r\n");

              display_setCursor(14, 51);
              display_setTextSize(1);
              display_printf("Canal 4\r\n");

              display();

               Code_ON = code4_on;
               Code_OFF = code4_off;
               break;
             };
   case 5 : {
              display_fillRoundRect(3, 20, 100, 40,2, BLACK);
              display_drawCircle(7, 24, 3, WHITE);
              display_drawCircle(7, 34, 3, WHITE);
              display_drawCircle(7, 44, 3, WHITE);
              display_fillCircle(7, 54, 3, WHITE);
              //Slider

              display_setCursor(14, 21);
              display_setTextSize(1);
              display_printf("Canal 2\r\n");

              display_setCursor(14, 31);
              display_setTextSize(1);
              display_printf("Canal 3\r\n");

              display_setCursor(14, 41);
              display_setTextSize(1);
              display_printf("Canal 4\r\n");

              display_setCursor(14, 51);
              display_setTextSize(1);
              display_printf("Canal 5\r\n");

              display();

               Code_ON = code5_on;
               Code_OFF = code5_off;
               break;
             };
 case 6 : {
              display_fillRoundRect(3, 20, 100, 40,2, BLACK);
              display_drawCircle(7, 24, 3, WHITE);
              display_drawCircle(7, 34, 3, WHITE);
              display_drawCircle(7, 44, 3, WHITE);
              display_fillCircle(7, 54, 3, WHITE);
              //Slider

              display_setCursor(14, 21);
              display_setTextSize(1);
              display_printf("Canal 3\r\n");

              display_setCursor(14, 31);
              display_setTextSize(1);
              display_printf("Canal 4\r\n");

              display_setCursor(14, 41);
              display_setTextSize(1);
              display_printf("Canal 5\r\n");

              display_setCursor(14, 51);
              display_setTextSize(1);
              display_printf("Canal 6\r\n");

              display();

               Code_ON = code6_on;
               Code_OFF = code6_off;
               break;
             };
 case 7 : {
              display_fillRoundRect(3, 20, 100, 40,2, BLACK);
              display_drawCircle(7, 24, 3, WHITE);
              display_drawCircle(7, 34, 3, WHITE);
              display_drawCircle(7, 44, 3, WHITE);
              display_fillCircle(7, 54, 3, WHITE);
              //Slider

              display_setCursor(14, 21);
              display_setTextSize(1);
              display_printf("Canal 4\r\n");

              display_setCursor(14, 31);
              display_setTextSize(1);
              display_printf("Canal 5\r\n");

              display_setCursor(14, 41);
              display_setTextSize(1);
              display_printf("Canal 6\r\n");

              display_setCursor(14, 51);
              display_setTextSize(1);
              display_printf("Canal 7\r\n");

              display();

               Code_ON = code7_on;
               Code_OFF = code7_off;
               break;
             };
case 8 : {
              display_fillRoundRect(3, 20, 100, 40,2, BLACK);
              display_drawCircle(7, 24, 3, WHITE);
              display_drawCircle(7, 34, 3, WHITE);
              display_drawCircle(7, 44, 3, WHITE);
              display_fillCircle(7, 54, 3, WHITE);
              //Slider

              display_setCursor(14, 21);
              display_setTextSize(1);
              display_printf("Canal 5\r\n");

              display_setCursor(14, 31);
              display_setTextSize(1);
              display_printf("Canal 6\r\n");

              display_setCursor(14, 41);
              display_setTextSize(1);
              display_printf("Canal 7\r\n");

              display_setCursor(14, 51);
              display_setTextSize(1);
              display_printf("Canal 8\r\n");

              display();

               Code_ON = code8_on;
               Code_OFF = code8_off;
               break;
             };
case 9 : {
              display_fillRoundRect(3, 20, 100, 40,2, BLACK);
              display_drawCircle(7, 24, 3, WHITE);
              display_drawCircle(7, 34, 3, WHITE);
              display_drawCircle(7, 44, 3, WHITE);
              display_fillCircle(7, 54, 3, WHITE);
              //Slider

              display_setCursor(14, 21);
              display_setTextSize(1);
              display_printf("Canal 6\r\n");

              display_setCursor(14, 31);
              display_setTextSize(1);
              display_printf("Canal 7\r\n");

              display_setCursor(14, 41);
              display_setTextSize(1);
              display_printf("Canal 8\r\n");

              display_setCursor(14, 51);
              display_setTextSize(1);
              display_printf("Canal 9\r\n");

              display();

               Code_ON = code9_on;
               Code_OFF = code9_off;
               break;
             };
case 10 : {
              display_fillRoundRect(3, 20, 100, 40,2, BLACK);
              display_drawCircle(7, 24, 3, WHITE);
              display_drawCircle(7, 34, 3, WHITE);
              display_drawCircle(7, 44, 3, WHITE);
              display_fillCircle(7, 54, 3, WHITE);
              //Slider

              display_setCursor(14, 21);
              display_setTextSize(1);
              display_printf("Canal 7\r\n");

              display_setCursor(14, 31);
              display_setTextSize(1);
              display_printf("Canal 8\r\n");

              display_setCursor(14, 41);
              display_setTextSize(1);
              display_printf("Canal 9\r\n");

              display_setCursor(14, 51);
              display_setTextSize(1);
              display_printf("Canal 10\r\n");

              display();

               Code_ON = code10_on;
               Code_OFF = code10_off;
               break;
             };
case 11 : {
              display_fillRoundRect(3, 20, 100, 40,2, BLACK);
              display_drawCircle(7, 24, 3, WHITE);
              display_drawCircle(7, 34, 3, WHITE);
              display_drawCircle(7, 44, 3, WHITE);
              display_fillCircle(7, 54, 3, WHITE);
              //Slider

              display_setCursor(14, 21);
              display_setTextSize(1);
              display_printf("Canal 8\r\n");

              display_setCursor(14, 31);
              display_setTextSize(1);
              display_printf("Canal 9\r\n");

              display_setCursor(14, 41);
              display_setTextSize(1);
              display_printf("Canal 10\r\n");

              display_setCursor(14, 51);
              display_setTextSize(1);
              display_printf("Canal 11\r\n");

              display();

               Code_ON = code11_on;
               Code_OFF = code11_off;
               break;
             };
case 12 : {
              display_fillRoundRect(3, 20, 100, 40,2, BLACK);
              display_drawCircle(7, 24, 3, WHITE);
              display_drawCircle(7, 34, 3, WHITE);
              display_drawCircle(7, 44, 3, WHITE);
              display_fillCircle(7, 54, 3, WHITE);
              //Slider

              display_setCursor(14, 21);
              display_setTextSize(1);
              display_printf("Canal 9\r\n");

              display_setCursor(14, 31);
              display_setTextSize(1);
              display_printf("Canal 10\r\n");

              display_setCursor(14, 41);
              display_setTextSize(1);
              display_printf("Canal 11\r\n");

              display_setCursor(14, 51);
              display_setTextSize(1);
              display_printf("Canal 12\r\n");

              display();

               Code_ON = code12_on;
               Code_OFF = code12_off;
               break;
             };
case 13 : {
              display_fillRoundRect(3, 20, 100, 40,2, BLACK);
              display_drawCircle(7, 24, 3, WHITE);
              display_drawCircle(7, 34, 3, WHITE);
              display_drawCircle(7, 44, 3, WHITE);
              display_fillCircle(7, 54, 3, WHITE);
              //Slider

              display_setCursor(14, 21);
              display_setTextSize(1);
              display_printf("Canal 10\r\n");

              display_setCursor(14, 31);
              display_setTextSize(1);
              display_printf("Canal 11\r\n");

              display_setCursor(14, 41);
              display_setTextSize(1);
              display_printf("Canal 12\r\n");

              display_setCursor(14, 51);
              display_setTextSize(1);
              display_printf("Canal 13\r\n");

              display();

               Code_ON = code13_on;
               Code_OFF = code13_off;
               break;
             };
    case 14 : {
                  display_fillRoundRect(3, 20, 100, 40,2, BLACK);
              display_drawCircle(7, 24, 3, WHITE);
              display_drawCircle(7, 34, 3, WHITE);
              display_drawCircle(7, 44, 3, WHITE);
              display_fillCircle(7, 54, 3, WHITE);
              //Slider

              display_setCursor(14, 21);
              display_setTextSize(1);
              display_printf("Canal 11\r\n");

              display_setCursor(14, 31);
              display_setTextSize(1);
              display_printf("Canal 12\r\n");

              display_setCursor(14, 41);
              display_setTextSize(1);
              display_printf("Canal 13\r\n");

              display_setCursor(14, 51);
              display_setTextSize(1);
              display_printf("Canal 14\r\n");

              display();

               Code_ON = code14_on;
               Code_OFF = code14_off;
               break;
             };
    case 15 : {
              display_fillRoundRect(3, 20, 100, 40,2, BLACK);
              display_drawCircle(7, 24, 3, WHITE);
              display_drawCircle(7, 34, 3, WHITE);
              display_drawCircle(7, 44, 3, WHITE);
              display_fillCircle(7, 54, 3, WHITE);
              //Slider

              display_setCursor(14, 21);
              display_setTextSize(1);
              display_printf("Canal 12\r\n");

              display_setCursor(14, 31);
              display_setTextSize(1);
              display_printf("Canal 13\r\n");

              display_setCursor(14, 41);
              display_setTextSize(1);
              display_printf("Canal 14\r\n");

              display_setCursor(14, 51);
              display_setTextSize(1);
              display_printf("Canal 15\r\n");

              display();

               Code_ON = code15_on;
               Code_OFF = code15_off;
               break;
             };
    case 16 : {
              display_fillRoundRect(3, 20, 100, 40,2, BLACK);
              display_drawCircle(7, 24, 3, WHITE);
              display_drawCircle(7, 34, 3, WHITE);
              display_drawCircle(7, 44, 3, WHITE);
              display_fillCircle(7, 54, 3, WHITE);
              //Slider

              display_setCursor(14, 21);
              display_setTextSize(1);
              display_printf("Canal 13\r\n");

              display_setCursor(14, 31);
              display_setTextSize(1);
              display_printf("Canal 14\r\n");

              display_setCursor(14, 41);
              display_setTextSize(1);
              display_printf("Canal 15\r\n");

              display_setCursor(14, 51);
              display_setTextSize(1);
              display_printf("Canal 16\r\n");

              display();

               Code_ON = code16_on;
               Code_OFF = code16_off;
               break;
             };
   }
}