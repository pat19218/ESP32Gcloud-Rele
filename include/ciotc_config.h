/******************************************************************************
 * Copyright 2018 Google
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/
// This file contains your configuration used to connect to Cloud IoT Core

// WIFI
const char* ssid = "<name_wifi>";
const char* password = "<PSWRD_wifi>";

// Cloud iot details.
const char* project_id = "<project_id>";
const char* location = "<Region>";
const char* registry_id = "<registry_id>";
const char* device_id = "<device_id>";

// Configuration for NTP
const char* ntp_primary = "time.google.com";
const char* ntp_secondary = "pool.ntp.org";

// To get the private key run (where private-key.pem is the ec private key
// used to create the certificate uploaded to google cloud iot):
// openssl ec -in <private-key.pem> -noout -text
// and copy priv: part.
// The key length should be exactly the same as the key length bellow (32 pairs
// of hex digits). If it's bigger and it starts with "00:" delete the "00:". If
// it's smaller add "00:" to the start. If it's too big or too small something
// is probably wrong with your key.
const char* private_key =
   "d8:1a:33:20:15:f1:37:cf:1f:39:22:c6:da:1e:82:"
   "52:a4:28:21:e5:87:54:10:77:fa:ff:73:78:d6:34:"
   "9b:e6";

// Time (seconds) to expire token += 20 minutes for drift
const int jwt_exp_secs = 36000; // Maximum 24H (3600*24)

// Certificates for SSL on the LTS server
const char* primary_ca = "-----BEGIN CERTIFICATE-----\n"
        "MIIBxTCCAWugAwIBAgINAfD3nVndblD3QnNxUDAKBggqhkjOPQQDAjBEMQswCQYD\n"
        "VQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzERMA8G\n"
        "A1UEANOT-CERTIFICATExMTNOT-CERTIFICATENOT-CERTIFICATEDAwMDQyWjBE\n"
        "MQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExM\n"
        "QzERMA8GA1UEAxMIR1RTIExUU1IwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAATN\n"
        "8YyO2u+yCNOT-CERTIFICATEkfULfrA6QJgFV2XMuENtQZIG5HUOS6jFn8f0ySlV\n"
        "eORCxqFyjDJyRn86d+Iko0IwQDAOBgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUw\n"
        "AwEB/zAdBgNVHQ4EFgQUPv7/zFLrvzQ+PfNA0OQlsV+4u1IwCgYIKoZIzj0EAwID\n"
        "SAAwRQIhAPKuf/VtBHqGw3TUwUIq7TfaEx85DBA34pBmVXJupT9FAiBr0SmCtsuk\n"
        "miGgpajjf/gFigGM34F9021bCWs1MbL0SA==\n"
        "-----END CERTIFICATE-----\n";

    
const char* backup_ca = "-----BEGIN CERTIFICATE-----\n"
        "MIIBxTCCAWugAwIBAgINAfD3nVndblD3QnNxUDAKBggqhkjOPQQDAjBEMQswCQYD\n"
        "VQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzERMA8G\n"
        "A1UEANOT-CERTIFICATExMTNOT-CERTIFICATENOT-CERTIFICATEDAwMDQyWjBE\n"
        "MQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExM\n"
        "QzERMA8GA1UEAxMIR1RTIExUU1IwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAATN\n"
        "8YyO2u+yCNOT-CERTIFICATEkfULfrA6QJgFV2XMuENtQZIG5HUOS6jFn8f0ySlV\n"
        "eORCxqFyjDJyRn86d+Iko0IwQDAOBgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUw\n"
        "AwEB/zAdBgNVHQ4EFgQUPv7/zFLrvzQ+PfNA0OQlsV+4u1IwCgYIKoZIzj0EAwID\n"
        "SAAwRQIhAPKuf/VtBHqGw3TUwUIq7TfaEx85DBA34pBmVXJupT9FAiBr0SmCtsuk\n"
        "miGgpajjf/gFigGM34F9021bCWs1MbL0SA==\n"
        "-----END CERTIFICATE-----\n";

// In case we ever need extra topics
const int ex_num_topics = 0;
const char* ex_topics[ex_num_topics];
//const int ex_num_topics = 1;
//const char* ex_topics[ex_num_topics] = {
//  "/devices/my-device/tbd/#"
//};