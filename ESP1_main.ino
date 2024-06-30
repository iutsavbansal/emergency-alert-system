

/**
 * Created by K. Suwatchai (Mobizt)
 *
 * Email: suwatchai@outlook.com
 *
 * Github: https://github.com/mobizt/ESP-Mail-Client
 *
 * Copyright (c) 2023 mobizt
 */

// This example shows how to send html message.

/** Note for library update from v2.x.x to v3.x.x.
 *
 *  Struct data names changed
 *
 * "ESP_Mail_Session" changes to "Session_Config"
 * "IMAP_Config" changes to "IMAP_Data"
 *
 * Changes in the examples
 *
 * ESP_Mail_Session session;
 * to
 * Session_Config config;
 *
 * IMAP_Config config;
 * to
 * IMAP_Data imap_data;
 */
#include <ezButton.h>
#include <Arduino.h>
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#elif __has_include(<WiFiNINA.h>)
#include <WiFiNINA.h>
#elif __has_include(<WiFi101.h>)
#include <WiFi101.h>
#elif __has_include(<WiFiS3.h>)
#include <WiFiS3.h>
#endif
#include <LiquidCrystal.h>

WiFiClient TCPclient;

// Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(D8, D5, D4, D3, D2, D1);

#include <ESP_Mail_Client.h>

#define WIFI_SSID "Utsav-PC"
#define WIFI_PASSWORD "utsavbansal"

const char* ssid = "Utsav-PC";     // CHANGE TO YOUR WIFI SSID
const char* password = "utsavbansal"; // CHANGE TO YOUR WIFI PASSWORD
const char* serverAddress = "192.168.137.223"; // CHANGE TO ESP32#2'S IP ADDRESS
const int serverPort = 4080;
ezButton button(D6); // create ezButton

/** For Gmail, the app password will be used for log in
 *  Check out https://github.com/mobizt/ESP-Mail-Client#gmail-smtp-and-imap-required-app-passwords-to-sign-in
 *
 * For Yahoo mail, log in to your yahoo mail in web browser and generate app password by go to
 * https://login.yahoo.com/account/security/app-passwords/add/confirm?src=noSrc
 *
 * To use Gmai and Yahoo's App Password to sign in, define the AUTHOR_PASSWORD with your App Password
 * and AUTHOR_EMAIL with your account email.
 */

/** The smtp host name e.g. smtp.gmail.com for GMail or smtp.office365.com for Outlook or smtp.mail.yahoo.com */
#define SMTP_HOST "smtp.gmail.com"

/** The smtp port e.g.
 * 25  or esp_mail_smtp_port_25
 * 465 or esp_mail_smtp_port_465
 * 587 or esp_mail_smtp_port_587
 */
#define SMTP_PORT 465

/* The log in credentials */
#define AUTHOR_EMAIL "alerts.iitgoa@gmail.com"
#define AUTHOR_PASSWORD "ksnpkfsesgcicvlz"

/* Recipient email address */
//#define RECIPIENT_EMAIL "fanfukra31@gmail.com"
//String RECIPIENT_EMAIL[2][2] = {{"Security Office","fanfukra31@gmail.com"},{"Utsav","text@abcd.xyz"}};
String RECIPIENT_EMAIL[2][2] = {{"Security Office","utsavbansal75@gmail.com"},{"Utsav","fanfukra31@gmail.com"}};

/* Declare the global used SMTPSession object for SMTP transport */
SMTPSession smtp;

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
WiFiMulti multi;
#endif

void setup()
{
  pinMode(D7,OUTPUT);
  pinMode(D6,INPUT);
  lcd.begin(16, 2);
  lcd.clear();

  Serial.begin(115200);

#if defined(ARDUINO_ARCH_SAMD)
  while (!Serial)
    ;
#endif

  Serial.println();

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
  multi.addAP(WIFI_SSID, WIFI_PASSWORD);
  multi.run();
#else
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
#endif

  Serial.print("Connecting to Wi-Fi");

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
  unsigned long ms = millis();
#endif

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    if (millis() - ms > 10000)
      break;
#endif
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  if (TCPclient.connect(serverAddress, serverPort)) {
    Serial.println("Connected to TCP server");
  } else {
    Serial.println("Failed to connect to TCP server");
  }

  
}
  
  

  /** The html text message character set e.g.
   * us-ascii
   * utf-8
   * utf-7
   * The default value is utf-8
   */

  /** The content transfer encoding e.g.
   * enc_7bit or "7bit" (not encoded)
   * enc_qp or "quoted-printable" (encoded)
   * enc_base64 or "base64" (encoded)
   * enc_binary or "binary" (not encoded)
   * enc_8bit or "8bit" (not encoded)
   * The default value is "7bit"
   */

  /** The message priority
   * esp_mail_smtp_priority_high or 1
   * esp_mail_smtp_priority_normal or 3
   * esp_mail_smtp_priority_low or 5
   * The default value is esp_mail_smtp_priority_low
   */

  /** The Delivery Status Notifications e.g.
   * esp_mail_smtp_notify_never
   * esp_mail_smtp_notify_success
   * esp_mail_smtp_notify_failure
   * esp_mail_smtp_notify_delay
   * The default value is esp_mail_smtp_notify_never
   */
  // message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;

  /* Set the custom message header */
  

  /* Set the TCP response read timeout in seconds */
  // smtp.setTCPTimeout(10);

  /* Connect to the server */

int switch_status_old = 0;
void loop()
{

  if (!TCPclient.connected()) {
    Serial.println("Connection is disconnected");
    TCPclient.stop();

    // reconnect to TCP server (Arduino #2)
    if (TCPclient.connect(serverAddress, serverPort)) {
      Serial.println("Reconnected to TCP server");
    } else {
      Serial.println("Failed to reconnect to TCP server");
    }
  }

//Serial.println("value of D6");
//Serial.println(digitalRead(D6));
//Serial.println("Yes");

  if(digitalRead(D6)==1){
    TCPclient.write('1');
    TCPclient.flush();
    //Serial.println("- The button is pressed, sent command: 1");

    digitalWrite(D7,HIGH);
    if (switch_status_old == 0){
      switch_status_old = 1;
      Serial.println("hello");
      sendEmail();
    }
    
  }
  else{

    TCPclient.write('0');
    TCPclient.flush();
    //Serial.println("- The button is released, sent command: 0");
    
    if (switch_status_old == 1){
      switch_status_old = 0;
    }
    digitalWrite(D7,LOW);
    lcd.clear();
    }
}

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status)
{
  /* Print the current status */
  Serial.println(status.info());

  /* Print the sending result */
  if (status.success())
  {
    // MailClient.printf used in the examples is for format printing via debug Serial port
    // that works for all supported Arduino platform SDKs e.g. SAMD, ESP32 and ESP8266.
    // In ESP8266 and ESP32, you can use Serial.printf directly.

    Serial.println("----------------");
    MailClient.printf("Message sent success: %d\n", status.completedCount());
    MailClient.printf("Message sent failed: %d\n", status.failedCount());
    Serial.println("----------------\n");

    for (size_t i = 0; i < smtp.sendingResult.size(); i++)
    {
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);

      // In case, ESP32, ESP8266 and SAMD device, the timestamp get from result.timestamp should be valid if
      // your device time was synched with NTP server.
      // Other devices may show invalid timestamp as the device time was not set i.e. it will show Jan 1, 1970.
      // You can call smtp.setSystemTime(xxx) to set device time manually. Where xxx is timestamp (seconds since Jan 1, 1970)

      MailClient.printf("Message No: %d\n", i + 1);
      MailClient.printf("Status: %s\n", result.completed ? "success" : "failed");
      MailClient.printf("Date/Time: %s\n", MailClient.Time.getDateTimeString(result.timestamp, "%B %d, %Y %H:%M:%S").c_str());
      MailClient.printf("Recipient: %s\n", result.recipients.c_str());
      MailClient.printf("Subject: %s\n", result.subject.c_str());
    }
    Serial.println("----------------\n");

    // You need to clear sending result as the memory usage will grow up.
    smtp.sendingResult.clear();
  }
}

void sendEmail() {

  lcd.print("Email is been");
  lcd.setCursor(0, 1);
  lcd.print("sending");
  /*  Set the network reconnection option */
  MailClient.networkReconnect(true);

  // The WiFi credentials are required for Pico W
  // due to it does not have reconnect feature.
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
  MailClient.clearAP();
  MailClient.addAP(WIFI_SSID, WIFI_PASSWORD);
#endif

  /** Enable the debug via Serial port
   * 0 for no debugging
   * 1 for basic level debugging
   *
   * Debug port can be changed via ESP_MAIL_DEFAULT_DEBUG_PORT in ESP_Mail_FS.h
   */
  

  for (int i = 0; i < sizeof(RECIPIENT_EMAIL) / sizeof(RECIPIENT_EMAIL[0]); i++) {
    smtp.debug(1);

    /* Set the callback function to get the sending results */
    smtp.callback(smtpCallback);

    /* Declare the Session_Config for user defined session credentials */
    Session_Config config;

    /* Set the session config */
    config.server.host_name = SMTP_HOST;
    config.server.port = SMTP_PORT;
    config.login.email = AUTHOR_EMAIL;
    config.login.password = AUTHOR_PASSWORD;

    /** Assign your host name or you public IPv4 or IPv6 only
    * as this is the part of EHLO/HELO command to identify the client system
    * to prevent connection rejection.
    * If host name or public IP is not available, ignore this or
    * use loopback address "127.0.0.1".
    *
    * Assign any text to this option may cause the connection rejection.
    */
    config.login.user_domain = F("127.0.0.1");

    /*
    Set the NTP config time
    For times east of the Prime Meridian use 0-12
    For times west of the Prime Meridian add 12 to the offset.
    Ex. American/Denver GMT would be -6. 6 + 12 = 18
    See https://en.wikipedia.org/wiki/Time_zone for a list of the GMT/UTC timezone offsets
    */
    config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
    config.time.gmt_offset = 3;
    config.time.day_light_offset = 0;

    /* The full message sending logs can now save to file */
    /* Since v3.0.4, the sent logs stored in smtp.sendingResult will store only the latest message logs */
    // config.sentLogs.filename = "/path/to/log/file";
    // config.sentLogs.storage_type = esp_mail_file_storage_type_flash;

    

    /* Set the message headers */
    /* Declare the message class */
    SMTP_Message message;
    message.sender.name = F("Emergency Alerts IIT Goa");
    message.sender.email = AUTHOR_EMAIL;
    message.subject = F("Emergency in Lab 1");
    message.addRecipient(RECIPIENT_EMAIL[i][0], RECIPIENT_EMAIL[i][1]);

    //String htmlMsg = "<p>Hi! there is an <span style=\"color:#ff0000;\">Emergency in the Lab 1</span></p><p>The message was sent via ESP device.</p>";
    String htmlMsg = "<p>This mail is to inform you that there is an <span style=\"color:#ff0000;\"><b>Emergency</b></span> in the <b>Lab 1</b></p><p>Please provide help as soon as possible</p><p style=\"color: gray;\"><em>Please note that this is computer generated mail<br>Don't reply back.</em></p>";
    message.html.content = htmlMsg;
    message.html.charSet = F("us-ascii");
    message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
    message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
    message.addHeader(F("Message-ID: alerts.iitgoa@gmail.com>"));

    if (!smtp.connect(&config))
    {
      MailClient.printf("Connection error, Status Code: %d, Error Code: %d, Reason: %s\n", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
      return;
    }

    if (!smtp.isLoggedIn())
    {
      Serial.println("Error, Not yet logged in.");
    }
    else
    {
      if (smtp.isAuthenticated())
        Serial.println("Successfully logged in.");
      else
        Serial.println("Connected with no Auth.");
    }

    /* Start sending Email and close the session */
    if (!MailClient.sendMail(&smtp, &message))
      MailClient.printf("Error, Status Code: %d, Error Code: %d, Reason: %s\n", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());

    // to clear sending result log
    // smtp.sendingResult.clear();

    MailClient.printf("Free Heap: %d\n", MailClient.getFreeHeap());
  }
  lcd.clear();
  lcd.print("Email is been");
  lcd.setCursor(0, 1);
  lcd.print("sent");
}