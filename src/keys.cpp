#include <keys.h>

static const int white_keys_amt = 14; //Valkosten koskettimien määrä
static bool keys_pressed[2 * white_keys_amt] { false }; //Tieto siitä, onko näppäintä painettu
static int octave = 5;
static float white_width() { return screen_width / white_keys_amt; }
static float white_height() { return screen_height / 3; }
static float black_width() { return white_width() * 0.5; }
static float black_height() { return white_height() * 0.8; }

static synth_t* synth;

void keys_init(synth_t *s)
{
  synth = s;
}

void keys_render()
{
  
  glPushMatrix();
  glTranslatef(0, screen_height - white_height(), 0);
  glBegin(GL_QUADS);

  int index = 0;
  //Piirretään valkoiset koskettimet
  for (int n = 0; n < white_keys_amt; n++)
    {
      if (!keys_pressed[index])
	glColor3f(1.0, 1.0, 1.0);
      else
	glColor3f(1.0, 1.0, 0.3);
      glVertex3f(n * white_width() + 1, 0, 0);
      glVertex3f((n + 1) * white_width() - 1, 0, 0);
      glVertex3f((n + 1) * white_width() - 1, white_height(), 0);
      glVertex3f(n * white_width() + 1, white_height(), 0);
      index++;
      if ((n % 7) != 2  && (n % 7) != 6)
      	index++;
    }

  index = 0;
  //Piirretään mustat koskettimet
  for (int n = 0; n < white_keys_amt; n++)
    {
      index++;
      if ((n % 7) != 2  && (n % 7) != 6)
	{
	  if (!keys_pressed[index])
	    glColor3f(0.0, 0.0, 0.0);
	  else
	    glColor3f(0.4, 0.4, 0.0);
	  float x = (n + 1) * white_width() - black_width() / 2;
	  glVertex3f(x, 0, 0);
	  glVertex3f(x + black_width(), 0, 0);
	  glVertex3f(x + black_width(), black_height(), 0);
	  glVertex3f(x, black_height(), 0);
	  index++;
	}

    }

  glEnd();
  glPopMatrix();
}

void keys_press(unsigned int key)
{
  if (key < 2 * white_keys_amt)
    {
      keys_pressed[key] = true;
      synth_play_key(synth, key + octave * 12);
    }
}

void keys_release()
{
  for (int n = 0; n < 2* white_keys_amt; n++)
    {
      keys_pressed[n] = false;
    }
}

/**
Palauttaa annetussa pisteessä olevan koskettimen järjestysnumeron.
Pisteen x ja y annetaan väliltä [0,1].
Mikäli pisteessä ei ole kosketinta, palautetaan -1.

Järjestysnumero kertoo siis monesko kosketin vasemmalta lasketttuna.
Huomattava, että myös olemattomat mustat koskettimet lasketaan mukaan,
elikä valkoiset koskettimet ovat 0, 2, 4, 6...
ja mustat koskettimet ovat 1, 3, 7, 9...
eikä funktion pitäisi koskaan palauttaa esim 5, koska siinä ei ole kosketinta.

Kaiken kaikkiaan siis hyvin ontuva ratkaisu.
 */
int get_key_at(float x, float y)
{
  x = x * screen_width;
  y = y * screen_height;
  if (y < screen_height - white_height() || y > screen_height || x < 0 || x > screen_width)
    return -1;

  //Lasketaan mikä valkoinen kosketin on kyseessä
  int result = (int)(x / white_width()) * 2;

  if (y <= screen_height - white_height() + black_height())
    {
      //Korkeuden perusteella kyseessä saattaa olla musta kosketin

      int x_within = x - (result / 2) * white_width(); //Sijainti näppäimen sisällä
      if (x_within >= white_width() - black_width() / 2)
	{
	  //Kyseessä onkin ylennetty nuotti, mikäli sellainen tästä kohdasta löytyy
	  if ((result + 1) % 14 != 5 && (result + 1) % 14 != 13)
	    {
	      result++;
	    }
	}
      else if(x_within <= black_width() / 2)
	{
	  //Kyseessä onkin alennettu nuotti, mikäli sellainen tästä kohdasta löytyy
	  if ((result - 1) > 0 && (result - 1) % 14 != 5 && (result - 1) % 14 != 13)
	    result--;
	}
    }



  if (result % 14 / 13 >= 1)
    result -= 2;
  else if (result % 14 / 5 >= 1)
    result -= 1;
  result -= result / 14 * 2;

  return result;
}
