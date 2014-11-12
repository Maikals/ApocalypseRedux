
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

const int CAOTIC = 1;
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

  typedef vector<int> VE;
  typedef vector<pair<int, bool> > Fila;
  typedef vector<Fila> Mapa;

  MEE personalitat; // Guarda la personalitat dels soldats entre torns.

  void prova(Mapa &m, int x, int y, queue <Posicio>& q, int d, int equip) {
  	if (valid(x,y)) {
  		if ((que(x,y) == GESPA or que(x,y) == BOSC) and not m[x][y].second) {
  			q.push(Posicio(x,y));
  			if (m[x][y].first == 0) m[x][y].first = d+1;
  		}
  	}
  }

  void moure(int id, Mapa& m, Posicio& p){
  	/*cerr << "x: " << p.x << " p.y " << p.y << endl;
  	for (int i = 0; i < MAX; ++i) {
  		for (int j = 0; j < MAX; ++j) cerr << m[i][j].first << " ";
  		cerr << endl;
  	}*/
  	while (m[p.x][p.y].first != 1) {
  		bool mogut = false;
  		for (int i = 0; i < 8 and not mogut; ++i) {
  			int xx = p.x + X[i];
		    int yy = p.y + Y[i];
		    if (m[xx][yy].first == m[p.x][p.y].first-1) {
		    	mogut = true;
		    	p.x = xx;
		    	p.y = yy;
		    }
  		}
  	}
  	ordena_soldat(id, p.x, p.y);
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
    		m[i][j].first = 0;
    		m[i][j].second = false;
    	}
    }
    queue<Posicio> q;
    q.push(Posicio(x,y));
   // cerr << "posicio inicial " << x << " " << y << endl;
    while (not q.empty()) {
    	Posicio p = q.front();
    	q.pop();
    	if (de_qui_post(p.x,p.y) != 0 and de_qui_post(p.x,p.y) != equip) {
    //		cerr << "post a " << p.x << " " << p.y << " i distancia " << m[p.x][p.y].first << endl;
    		moure(id, m, p);
    		return;
    	}
    	else if (not m[p.x][p.y].second) {
    		m[p.x][p.y].second = true;
    		int d = m[p.x][p.y].first;
		    for (int i = 0; i < 8; ++i) {
		      int xx = p.x + X[i];
		      int yy = p.y + Y[i];
			  prova(m, xx, yy, q, d, equip);

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

