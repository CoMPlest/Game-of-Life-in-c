## Játékszabályok és a szimuláció menete
A játékban egy populációt szimulálunk.
A szabályok egyszerűek: 
- Adott egy valamilyen méretű négyzethálóra felosztott sík amelyekben a négyzetek sejteket jelölnek.
- A sejteknek 2 egyszerű állapotuk lehet: vagy halottak vagy éppen élnek. A kezdő állapotuk adott.
- Minden sejt a következő lépésben amelynek 3x3-as környezetében, ha:
	- több mint 3 élő sejt van, túlnépesedés miatt meghal.
	- 3 élő sejt van, feléled vagyis ha élő volt  élő marad, ha halott volt akkor élő lesz.
	- 2 élő sejt van, akkor változatlan marad az állapota.
	- 2-nél kevesebb élő sejt van, meghal.

# Felhasználói dokumentáció

### Lépések:
- Nyissunk meg egy konzolt. Lehet bármilyen pl.: `cmd`
- Navigáljunk a programunkhoz
- A bal felső sarokra jobb klikkelve keressük meg az ablak méreteinek beállítását és állítsuk be olyan szélességűre amilyen széles játékot akarunk futtatni
- Hívjuk meg a programunkat

Argumentumok:
```
gameoflife.exe <szelesseg> <hosszusag> <kezdoallapot_fajl> <kimeneti_fajl> <iteracioszam> 
```
A szelesség és a hosszúság argumentum szükséges a program futtatásához. Amennyiben ezeket nem adjuk meg a program kérdések formájában elkéri ezeket a felhasználótól

A kezdoallapot_fajl beállításával megadhatunk egy kezdőállapotot. Amennyiben ez nem tesszük meg a program egy véletlenszerűen generált kezdőpozíciót fog választani.

A kimeneti fájl megadásával megadhatjuk annak a fájlnak az elérési útját ahova a program futásának befejezésével lementi az utolsó állapotot

Az iterácioszam megadása esetén a program grafikus megjelenítés nélkül fog futni és a végén lementi a kimeneti fájlba az állapotot

# Programozói dokumentáció

## A játék logikája
### Adatszerkezetek
A program fő célja hogy egy sík 2 dimenziós tér keretein belül sejteket szimuláljon. Mivel az elvárások szerint a játéktér fix méretű, tehát a szimuláció alatt nem változik, a céljaink elérésére, és a sejtek adatainak tárolására teljesen tökéletesen megfelel egy átlagos két dimenziós statikus tömb amelyet dinamikusan tárolunk hogy tetszőleges méretű lehessen. A választható méretek miatt szükségünk lesz tehát egy szerkezetre amely tárolja a sejteket és a játéktér méreteit. Az egyszerűség kedvéért és azért hogy ne keljen minden sornak `malloc`-al külön külön memóriát foglalni, a tömb **1 dimenziós** lesz (emiatt a tömb mérete `width * height`).
A mi esetünkben ez a `struct GameState`:
```c
typedef struct GameState {  
    int width;  
    int height;  
    bool* cells;  
} GameState;
```

> [!note]
> Mivel a sejtekről csak egy 1 bites információt tárolunk (hogy éppen él vagy sem), teljesen elegendő egy "1 bites" boolean tömbben tárolnunk.

Továbbá szükségünk lesz egy adatszerkezetre amellyel koordinátákat tárolunk, hogy megkönnyítsük a függvények közötti kommunikációt, és hogy egyszerűbb legyen a koordinátákkal dolgozni.
A mi esetünkben ez a ` struct Vector`:
```c
typedef struct Vector {  
    int x;  
    int y;  
} Vector;
```

### Függvények

#### A fönti adatszerkezetek mellé szükségünk lesz segéd függvényekre:

```c
int getCellIndex(Vector coords, int width);
```
Átalakít egy `struct Vector`-t arra az indexre ami megfeleltethető az adott koordináták helyén lévő sejtnek. Röviden: Átmenetet képez `Vector` és a `GameState`-ben tárolt `bool* cells` között.

> [!important]
>  Az egyszerűség kedvéért egyenlőre egy 1 dimenziós tömbben tárlojuk a sejteket, emiatt van szükség erre a függvényre, hogy átalakítsunk egy 2 dimenziós koordinátát egy indexre


```c
GameState* createNewState(int width, int height);  
```
Dinamikus memória kezeléssel létrehoz egy `GameState` -et a megadott hosszúság és szélesség szerint majd visszaad egy pointert ami erre mutat.
Fontos hogy alapból nem nullázza ki az összes sejt adatát, hanem memória szemét van még a tömbben.


```c
void destroyGameState(GameState* state);
```
A `createNewState` el létrehozott `GameState` dinamikusan lefoglalt memóriáját felszabadítja.


```c
void clearCells(GameState* game);
```
Kinullázza egy `GameState` cell tömbjét.


```c
void randomizeCells(GameState* game);
```
Random `bool`-okkal feltölti a `GameState` cell tömbjét.


> [!info]
> A random függvény használata előtt meg kell hívni az `srand`-ot:
> ```c
> srand(time(NULL));
> ```
> Ez teljes mértékben a függvényt felhasználóra van bízva

#### További a játékot irányító függvények:

```c
int countAliveNeighbours(Vector* pos, GameState* game);
```
Egy `GameState`-ben megszámolja a `pos` koordinátán lévő sejt 3x3 as környezetében az élő sejteket.


```c
GameState* calculateNextState(GameState* game);
```
Egy adott `GameState`-nek visszaadja következő iterációját, a fenti szabályokat alkalmazva, egy új dinamikusan foglalt `GameState`-ben.

> [!Important]
> A memóriaszivárgást elkerülendő fontos hogy amikor új `GameState` et hozunk létre, a régit a `destroyGameState` el töröljük


```c
void stepGame(GameState** game);
```
Egy `GameState` pointert tovább léptet a játék következő állására, a régi állást pedig törli


```c
void convertToChar(GameState* game, char* buff)
```
Betölti egy bufferba a játékállást


```c
void loadStateFromFile(char* fileName, GameState* game);
```
Betölti a játékállást egy fájlból


```c
void saveStateToFile(char* fileName, GameState* game);
```
Elmenti a játékállást egy fájlba

---


## A játék megjelenítése
### Adatszerkezetek
A játék logikája mellett a megjelenítéssel is foglalkoznunk kell. A játék tesztelésénél felmerült az a probléma hogy az eredeti egyszerű `printf` függvény nem elég gyors ahhoz hogy a játékot nagyobb méreteknél normális meg lehessen jeleníteni. Ezért a játék közvetlenül a [win32 api](https://learn.microsoft.com/en-us/windows/console/console-functions)
console részével kommunikál. Ez elsőre bonyolultnak tűnhet, azonban a microsoft remek dokumentációt kínál fel számunkra. Viszont ez a függvények felhasználását illetően irrelevánsak. A windows api-val való kommunikáció megkönnyítésére egy `struct` ot hoztuk létre.
Ez a `struct Screen`:
```c
typedef struct Screen {  
    char* chars;  
    int size;  
    char* title;  
    HANDLE console;  
    DWORD bytesOnScreen;  
    CONSOLE_CURSOR_INFO conCurInf;  
} Screen;
```

### Függvények
```c
Screen* createScreen(int size, char* title)
```
Dinamikus memória kezeléssel létrehoz egy `Screen` -t a megadott méret szerint majd visszaad egy pointert ami erre mutat.


```c
void destroyScreen(Screen* screen);
```
Dinamikusan kezeli a `Screen` memóriájának felszabadítását


```c
void print2d(char* buff, int length, bool withLineBreak, int lineWidth);
```
Kiírja a buff-ban található karaktereket a képernyőre. Lassú!


```c
void render2d(Screen* screen);
```
A `Screen` char tömbjét a windows api segítségével megjeleníti a konzolon. Hátránya hogy nem lehet sortöréseket használni, azonban nagyon gyors és hatékony. Előtte hívjuk meg az aktiváló függvényt.


```c
void hideCursor(Screen* screen);
void showCursor(Screen* screen);
```
Eltünteti és megjeleníti a konzolban a kurzort


```c
void activateScreen(Screen* screen);
void deactivateScreen(Screen* screen);
```
Aktiválja illetve deaktiválja a létrehozott `Screen`-t. Fontos hogy a render függvény előtt aktiváljuk a `Screen`-t.


---


## A játék beállítása
### Adatszerkezetek
Úgy ahogyan a megjelenítésnél is könnyített a függvények közötti kommunikációt egy adatszerkezet itt is hasznunkra lesz egy pár.
A beállítások kezelésére ez a `struct Settings`:
```c
typedef struct Settings {  
    char *title;  
    int escapeKey;  
    int fps;  
    int maxFileNameLength;  
    char randomStartChar;  
    char noSaveChar;  
} Settings;
```
Illetve a program paramétereinek kezelésére a `struct Params`:
```c
typedef struct Params {  
    int width;  
    int height;  
    char *loadFileName;  
    char *saveFileName;  
    int iterationCount;  
} Params;
```
Így egy struct-ba össze tudjuk gyűjteni az összes beállítást és egy helyen kell csak módosítanunk ezeket ha más beállításokat szeretnénk.

### Függvények
```c
void questionUser(Params* params, Settings* settings);  
```
A kód olvashatósága érdekében jött létre, egy segéd függvény amely a konzolbemenet segítségével megkérdezi a felhasználót milyen paraméterekkel akarja futtatni a programot és betölti ezeket a `Params* param`-ba


```c
void parseArguments(int argc, char** argv, Params* params);
```
A program meghívásánál megadott argumentumokat betölti a `Params* parms` - ba.

