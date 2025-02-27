# Tank Wars

## Descriere
**Tank Wars** este un joc inspirat din conceptul clasic de luptă între tancuri. În cadrul acestui proiect, 
am implementat un joc cu gameplay interactiv, unde tancurile se deplasează pe un teren generat dinamic, 
ținând cont de forma acestuia. Proiectul folosește OpenGL și C++ pentru a crea o experiență de joc 
vizuală și interactivă.

## Cerințe
Pentru a rula acest proiect, ai nevoie de:
- **OpenGL** pentru redarea graficii
- **GLM** pentru manipularea matematicii 3D
- **C++** și un compilator compatibil cu OpenGL (ex. Visual Studio)
- **Framework-ul de laborator** utilizat pentru construcția terenului și manipularea tancurilor

## Funcționalități
### 1. **Teren Dinamic**
Terenul este generat folosind o combinație de sinusoide și parabole, iar înălțimea acestuia este
stocată într-o hartă de înălțime. Terenul poate fi deformabil în urma impactului cu proiectilele
lansate de tancuri.

### 2. **Controlul Tancurilor**
Tancurile se pot deplasa folosind tastele `A/D` sau săgețile stânga/dreapta. Turela tancului se 
rotește cu tastele `W/S` sau săgețile sus/jos, iar tancurile se orientează automat pe teren pentru
a se alinia cu panta acestuia.

### 3. **Proiectile**
Proiectilele sunt lansate din turela tancului, urmând o traiectorie parabolică. Impactul proiectilului 
cu terenul sau cu un alt tanc va genera efecte vizuale și va provoca daune tancului lovit.

### 4. **Detecția Coliziunii**
Coliziunile între proiectile și teren sunt determinate pe baza distanței față de punctele de pe teren.
În cazul unui impact, terenul va suferi o deformare locală.

### 5. **Interfața cu utilizatorul**
   - **Bara de viață**: Fiecare tanc va avea o bară de viață, care scade atunci când tancul este lovit de proiectile.
   - **Previzualizare traiectorie**: O linie de ghidaj va arăta traiectoria proiectilului înainte de a fi lansat.

## Structura Proiectului
### Fisierele principale:
- **main.cpp**: Punctul de intrare în joc.
- **lab3.cpp** și **lab3.h**: Conțin funcțiile de implementare pentru desenarea și manipularea obiectelor de joc.
- **object2D.cpp** și **object2D.h**: Definirea geometriei tancurilor și a proiectilelor.
- **transform2D.h**: Funcții pentru transformările geometrice (translație, scalare, rotație).
- **lab3_vis2D.cpp** și **lab3_vis2D.h**: Implementarea vizualizării 2D a jocului.

## Instalare
1. Clonează repository-ul:
   ```bash
   git clone git@github.com:enache-albertina/Tank-Wars.git
Deschide proiectul în Visual Studio sau orice alt IDE compatibil cu C++.
Compilează și rulează jocul.
Instrucțiuni de utilizare


Deplasarea tancurilor: Folosește tastele A și D sau săgețile stânga/dreapta pentru a deplasa tancul.
Controlul turelei: Folosește tastele W și S sau săgețile sus/jos pentru a roti turela.
Lansarea proiectilului: Apasă Space sau Enter pentru a lansa un proiectil.

