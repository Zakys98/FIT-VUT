# ISA 2020: Odpovědní arch pro cvičení č. 2

## Vzdálený terminál - SSH, Secure Shell

### (2.) Bezpečné připojení na vzdálený počítač bez autentizačních klíčů

*Verze OpenSSH*: 7.4

*Vybraná podporovaná šifra*: chacha20-poly1305@openssh.com

*Co lze zjistit o obsahu komunikace?*:
Po zobrazení TCP streamu dokážu zjistit následovné věci:
	- podporované šifry
	- verzi SSH
	- autentický mechanismus HMAC
	- mechanismus pro výměnu klíčů

### (3.) Vytvoření veřejného a privátního klíče

*Jak se liší práva mezi souborem s privátním a veřejným klíčem?*:
-rw------- id_rsa
-rw-r--r-- id_rsa.pub
Do privátní klíče smí zapisovat a číst ho pouze vlastník. Zatím co veřejný klíč mohou číst všichni a zapisovat do něho může pouze vlastník.

### (4.) Distribuce klíčů

*Jaká hesla bylo nutné zadat při kopírovaní klíčů?*: user4lab pro usera a pro roota root4lab. Po přidání souborů u usera bylo potřeba zadat fitvutisa a u roota nic.

*Jaká hesla bylo nutné zadat při opětovném přihlášení?*: fitvutisa

### (6.) Pohodlné opakované použití klíče

*Museli jste znovu zadávat heslo?*: Minimálně jednou ano. Po zaškrtnutí políčka na zapamatování hesla, už ne.

## Zabezpečení transportní vrstvy - TLS, Transport Layer Security

### (1.) Nezabezpečený přenos dat

*Je možné přečíst obsah komunikace?*: Ano

### (2.) Přenos dat zabezpečený TLS

*Je možné přečíst obsah komunikace?*: Ne
