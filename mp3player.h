#ifndef MP3PLAYER_H
#define MP3PLAYER_H

#include <QMainWindow>
#include <QObject>
#include <QProcess>
#include <QX11EmbedContainer>
#include <QTimer>
#include <QKeyEvent>
#include <QCoreApplication>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <wiringPiI2C.h>
#include <dirent.h>
#include <time.h>
//#include <X11/Xlib.h>
//#include <X11/keysym.h>
//#include <gtk/gtk.h>

#define ACTIVE_PIN 18
#define READY_PIN 27
#define SHUTDOWN_PIN 17
#define S_PIN 10

#define USB_1  '1'
#define USB_2  '2'
#define USB_3  '3'
#define USB_4  '4'
#define USB_5  '5'
#define USB_6  '6'
#define USBnext  'n'
#define USBprev  'p'
#define USBeject  'e'
#define USBtim  't'
#define USBinfo  'i'
#define USBas  'a'
#define USBscan  's'
#define USBflag  'f'
#define USBlight  'l'
#define USBnavi  'N'
#define USBback  'b'
#define USBaudio  'A'
#define USBtone  'T'
#define USBenter 'E'
#define USBtraffic 'v'
#define USBplus '+'
#define USBminus '-'


//Status definitionen
#define OFF 0;
#define MP3PLAYER 1;
#define NAVIT 2;
#define I2C 3;

#define MUSICDIR "/var/lib/mpd/music/"


struct node {
    struct node *left;
    struct node *right;
    char data[64];
};

enum status_t{
    status_off = 0,
    status_mp3player = 1,
    status_i2c = 3,
    status_navit = 2
};


namespace Ui {
class mp3player;
}

class mp3player : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit mp3player(QWidget *parent = 0);
    ~mp3player();
    struct node* load_playlists(void);

    //static void mpc(char* cmd);
   // void mpc(char command);

    struct node * new_node_list(char entry[64]);
    struct node * insert_right(struct node *list, char data[64]);
    struct node * insert_left(struct node *list, char data[64]);
    struct node* insert_sorted(struct node* list, char data[64]);
    struct node * delete_node(struct node *list);
    void print_all(struct node* list);
    struct node * restore(struct node *list);
    struct node* sort(struct node* list);
    void swap_nodes(struct node* a, struct node* b);
    struct node* prev_playlist(struct node* current);
    struct node* next_playlist(struct node* current);
    struct node* prev_artist(struct node* current);
    struct node* next_artist(struct node* current);
    struct node* get_entry(struct node* head, char data[64]);
    struct node* check_playlists(void);
    //<void print_song(void);
    struct node* right_node(struct node* list);
    struct node* left_node(struct node* list);
    char* get_data(struct node* entry);
    struct node* delete_and_free(struct node* list);
    struct node* delete_playlist(struct node* list);
    void next_song(void);
    void start_navit();
    //void resize_label_text(QString text);
    //void mp3_task(int ser);
    int mp3init(void);
    //XKeyEvent createKeyEvent(Display *display, Window &win, Window &winRoot, bool press,int keycode, int modifiers);
    struct node* get_last_playlist(void);
    void save_playlist(struct node* list);
    void delete_all_playlists(struct node* list);
    int ser;
    int no_ser_count;
    std::string radio_string;
    QProcess* navit;
    struct node* PlayListHead;
    struct node* CurrentPlayList;
    enum status_t status;
    unsigned char backcount;
    unsigned char ff_count;
    unsigned char rx_old;
    int delaycnt;
public slots:
    void mp3_task(void);
    void print_song(void);

private:
    Ui::mp3player *ui;

};

#endif // MP3PLAYER_H
