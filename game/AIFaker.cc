
#include "Player.hh"

using namespace std;


/**
 * Escriu el nom * del teu jugador i guarda 
 * aquest fitxer amb el nom AI*.cc
 */
#define PLAYER_NAME Faker


/**
 * Podeu declarar constants aquí
 */

const int CAOTIC = 2;
const int CICLIC = 2;

const int X[8] = { 1, 1, 0, -1, -1, -1,  0,  1 };
const int Y[8] = { 0, 1, 1,  1,  0, -1, -1, -1 };



struct PLAYER_NAME : public Player {


  /**
   * Factory: retorna una nova instància d'aquesta classe.
   * No toqueu aquesta funció.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }


  /**
   * Els atributs dels vostres jugadors es poden definir aquí.
   */     

  struct bfs {
    string dir;
    int d;
    bool v;
  };

  typedef vector<int> VE;
  typedef vector<bfs> Fila;
  typedef vector<Fila> Mapa;

  MEE personalitat; // Guarda la personalitat dels soldats entre torns.

  void prova(Mapa &m, int x, int y, queue <Posicio>& q, int d, int equip, string dir) {
  	if (valid(x,y)) {
  		if ((que(x,y) == GESPA or que(x,y) == BOSC) and not m[x][y].v) {
  			q.push(Posicio(x,y));
  			m[x][y].d = d+1;
        if (m[x][y].dir == " ") m[x][y].dir = dir;
  		}
  	}
  }
  
  void moure(int id, Mapa& m, Posicio& p){
  	/*cerr << "x: " << p.x << " p.y " << p.y << endl;
  	for (int i = 0; i < MAX; ++i) {
  		for (int j = 0; j < MAX; ++j) cerr << m[i][j].first << " ";
  		cerr << endl;
  	}*/
    Info in = dades(id);
  	if (m[p.x][p.y].dir == "N")	ordena_soldat(id, in.pos.x-1, in.pos.y);
    if (m[p.x][p.y].dir == "E") ordena_soldat(id, in.pos.x, in.pos.y+1);
    if (m[p.x][p.y].dir == "S") ordena_soldat(id, in.pos.x+1, in.pos.y);
    if (m[p.x][p.y].dir == "O") ordena_soldat(id, in.pos.x, in.pos.y-1);
    if (m[p.x][p.y].dir == "NO") ordena_soldat(id, in.pos.x-1, in.pos.y-1);
    if (m[p.x][p.y].dir == "NE") ordena_soldat(id, in.pos.x-1, in.pos.y+1);
    if (m[p.x][p.y].dir == "SO") ordena_soldat(id, in.pos.x+1, in.pos.y+1);
    if (m[p.x][p.y].dir == "SE") ordena_soldat(id, in.pos.x+1, in.pos.y-1);
  }

  void juga_soldat(int equip, int id) {
    Info in = dades(id);
    int x = in.pos.x;
    int y = in.pos.y;
    for (int i = 0; i < 8; ++i) {
      int xx = x + X[i];
      int yy = y + Y[i];
      if (valid(xx,yy)) {
    		int id2 = quin_soldat(xx, yy);
    		// Si tenim un enemic al costat, l'ataquem.
    		if (id2 and dades(id2).equip != equip) {
    		  ordena_soldat(id, xx, yy);
    		  return;
    		}
  	  }
  	}
    Mapa m (MAX, Fila(MAX));
    for (int i = 0; i < MAX; ++i) {
    	for (int j = 0; j < MAX; ++j) {
    		m[i][j].d = 0;
    		m[i][j].v = false;
        m[i][j].dir = " ";
    	}
    }
    queue<Posicio> q;
    prova(m, x-1, y, q, 0, equip, "N");
    prova(m, x, y+1, q, 0, equip, "E");
    prova(m, x+1, y, q, 0, equip, "S");
    prova(m, x, y-1, q, 0, equip, "O");
    prova(m, x-1, y-1, q, 0, equip, "NO");
    prova(m, x-1, y+1, q, 0, equip, "NE");
    prova(m, x+1, y+1, q, 0, equip, "SE");
    prova(m, x+1, y-1, q, 0, equip, "SO");
    while (not q.empty()) {
    	Posicio p = q.front();
    	q.pop();
    	if (de_qui_post(p.x,p.y) != 0 and de_qui_post(p.x,p.y) != equip) {
    	moure(id, m, p);
    		return;
    	}
    	else if (not m[p.x][p.y].v) {
    		m[p.x][p.y].v = true;
    		int d = m[p.x][p.y].d;
        string dir = m[p.x][p.y].dir;
		    for (int i = 0; i < 8; ++i) {
		      int xx = p.x + X[i];
		      int yy = p.y + Y[i];
			  prova(m, xx, yy, q, d, equip, dir);

	    	}
	    }
    }
  }


  void juga_heli(int equip, int id) {
    Info in = dades(id);
    int x = in.pos.x;
    int y = in.pos.y;
    int suma = 0;
    for (int i = -ABAST; i <= ABAST; ++i)
      for (int j = -ABAST; j <= ABAST; ++j) {
	int xx = x + i;
	int yy = y + j;
	if (xx >= 0 and xx < MAX and yy >= 0 and yy < MAX) {
	  int id2 = quin_soldat(xx, yy);
	  if (id2 and dades(id2).equip != equip) ++suma;
	}
      }
    // Si tenim 3 o més enemics a sota, intentem llençar napalm.
    if (suma >= 3 and dades(id).napalm == 0) {
      ordena_helicopter(id, NAPALM);
      return;
    }

    // Amb probabilitat 20% girem en el sentit contrari de les agulles del rellotge,
    // altrament intentem avançar dos passos.
    int c = uniforme(1, 5);
    ordena_helicopter(id, c == 1 ? CONTRA_RELLOTGE : AVANCA2);
  }


  void llenca_paracaigudista(int heli_id) {
    // Obtenim les dades de l'helicòpter...
    Info in = dades(heli_id);
    // ... i intentem llencar un paracaigudista, sense ni mirar el terreny.
    int xx = in.pos.x + uniforme(-2, 2);
    int yy = in.pos.y + uniforme(-2, 2);
    if (xx >= 0 and xx < MAX and yy >= 0 and yy < MAX)
      ordena_paracaigudista(xx, yy);
  }

  /**
   * Mètode play.
   * 
   * Aquest mètode serà invocat una vegada cada torn.
   */     
  virtual void play () {

    int equip = qui_soc();
    VE H = helis(equip);   // helicòpters del meu equip
    VE S = soldats(equip); // soldats del meu equip

    // si en un helicòpter escollit a l'atzar tinc paracaigudistes, en llenço un.
    int heli_id = H[uniforme(0, H.size()-1)];
    if (not dades(heli_id).paraca.empty()) llenca_paracaigudista(heli_id);

    for (int i = 0; i < (int)H.size(); ++i) juga_heli(equip, H[i]);
    for (int i = 0; i < (int)S.size(); ++i) juga_soldat(equip, S[i]);
  }

    
};


/**
 * No toqueu aquesta línia.
 */
RegisterPlayer(PLAYER_NAME);

