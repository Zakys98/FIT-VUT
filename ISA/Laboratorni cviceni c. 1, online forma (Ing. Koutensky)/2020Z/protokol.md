10# ISA 2020: Odpovědní arch pro cvičení č. 1

## Zjišťování konfigurace

### (1.) Rozhraní enp0s3

*MAC adresa*: 08:00:27:86:a0:b5

*IPv4 adresa*: 10.0.2.15

*Délka prefixu*: 24

*Adresa síťe*: 10.0.2.0

*Broadcastová adresa*: 10.0.2.255

### (2.) Výchozí brána

*MAC adresa*: 52:54:00:12:35:02

*IPv4 adresa*: 10.0.2.2

### (4.) DNS servery

*Soubor*: resolv.conf

*DNS servery*: 147.229.191.143, 147.229.190.143

### (5.) Ping na výchozí bránu

*Soubor*: /etc/hosts

*Úprava*: 10.0.2.2 gw

### (6.) TCP spojení

*Záznam + popis*:

State	Recv-Q	Send-Q	Local Address:Port 		Peer adress:Port
ESTAB	0		0		10.0.2.15:54762			195.113.232.73:https

State - stav spojení
Recv-Q - Počet bajtů nekopírovaných uživatelským programem připojeným k tomuto soketu.
Send-Q - Počet bajtů, které vzdálený hostitel nepotvrdil.
Local Address:Port - ip adresa a port lokálního zařízení
Peer adress:Port - ip adresa a port koncového zařízení

### (8.) NetworkManager události

*Příkaz*: sudo journalctl -u NetworkManager

### (9.) Chybová hláška sudo

*Příkaz*: sudo wireshark

*Chybová hláška*: user :  command not allowed ; TTY=pts/0 ; PWD=/home/user ; USER=root ; COMMAND=/bin/wireshark

## Wireshark

### (1.) Capture filter

*Capture filter*: port 80

### (2.) Zachycená HTTP komunikace

Komu patří nalezené IPv4 adresy a MAC adresy? Klientovi(mému pc) a serveru popřípadě MAC adresa brány(gateway)
Vypisovali jste již některé z nich? Ano
Proč tomu tak je? Příjemce reprezentuje enp0s3(můj pc). Jeho MAC adresa a ip adresa se nachází výše.
Cílová ip adresa je adresa webové stránky, ale její MAC adresa patří výchozí bráně, která se taky nachází výše.

#### Požadavek HTTP

Cílová MAC adresa

  - *Adresa*: 52:54:00:12:35:02
  - *Role zařízení*: brána

Cílová IP adresa

  - *Adresa*: 147.229.177.179
  - *Role zařízení*: server

Zdrojová MAC adresa

  - *Adresa*: 08:00:27:86:a0:b5
  - *Role zařízení*: klient

Zdrojová IP adresa

  - *Adresa*: 10.0.2.15
  - *Role zařízení*: klient


#### Odpověď HTTP

Cílová MAC adresa

  - *Adresa*: 08:00:27:86:a0:b5
  - *Role zařízení*: klient

Cílová IP adresa

  - *Adresa*: 10.0.2.15
  - *Role zařízení*: klient

Zdrojová MAC adresa

  - *Adresa*: 52:54:00:12:35:02
  - *Role zařízení*: brána

Zdrojová IP adresa

  - *Adresa*: 147.229.177.179
  - *Role zařízení*: server

### (3.) Zachycená ARP komunikace

*Display filter*: arp or icmp

### (6.) Follow TCP stream 

Jaký je formát zobrazených dat funkcí *Follow TCP stream*, slovně popište
význam funkce *Follow TCP stream*:
Aplikuje Disply filtr, který zobrazí stream komunikace.
Tato komunikace je mezi klientem a serverem. Komunikace je buď šifrovaná nebo nešifrovaná v textové podobě.
Je to posloupně, tudíž vidíme dotazy a odpovědi seřazené hezky za sebou.
