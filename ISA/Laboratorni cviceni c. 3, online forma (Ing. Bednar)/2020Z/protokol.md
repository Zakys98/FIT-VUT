# ISA 2020: Odpovědní arch pro cvičení č. 3

## (1.) Resolving DNS dotazů

Jaké jsou autoritativní DNS servery pro doménu *vutbr.cz*?\
   rhino.cis.vutbr.cz \
   pipit.cis.vutbr.cz \
*Display filter* pro zobrazení pouze DNS provozu: dns

Počet zachycených paketů souvisejících s vyhledáním NS pro doménu *vutbr.cz*: 2

Provedený DNS dotaz (vyberte správnou variantu): **rekurzivní**

Podle čeho jste zjistili zakroužkovaný typ DNS dotazu v zachyceném paketu? podle flagu RD(recursion desired) v dotazu

Cílová IP adresa paketu s DNS dotazem: 8.8.8.8

Jakému zařízení náleží zapsaná IP adresa? lokální DNS server(Google)


## (2.) Zabezpečení a resolving pomocí DNS over HTTPS

Dokážete zjistit ze zachyceného DNS provozu, jaké domény jste předtím navštívili? Proč?   
   Ne, protože komunikace jde přes šifrovaný protokol HTTPS.
  \
  \
*Display filter* pro zobrazení pouze TLS provozu: tls

Jeden řádek z položky *Answers* z libovolné DoH odpovědi:  
   sznplayer.cz: type AAAA, class IN, addr 2a02:598:2::1129 \
IP adresa, na kterou směřovaly pakety s DoH dotazem: 185.43.135.1

Doménové jméno patřící k doplněné IP adrese: odvr.nic.cz


## (3.) Zabezpečení a resolving pomocí DNS over TLS

*Display filter* pro zobrazení pouze provozu využívající TCP port 853: tcp.port == 853

*Display filter* pro zobrazení pouze provozu využívající TCP nebo UDP port 53: tcp.port == 53 or udp.port == 53

Služba běžící nad portem 53: DNS

Počet zachycených paketů se zdrojovým nebo cílovým portem 53: Žadný a protože to jde přes unbound, který to šifrovaně posílá na Cloudflare servery


## (4.) Blokování reklam a další



Jaký rozdíl jste zpozorovali na webu *idnes.cz* při jeho načtení s aktivním nástrojem *pi-hole*?
Stránka se načetla bez reklam, protože pihole je vyfiltroval.
