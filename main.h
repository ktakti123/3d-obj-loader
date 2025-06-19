// includes and defines 
  #include <stdio.h> 
  #include<windows.h>
  #include<stdbool.h>
  #include<math.h>
  #pragma once
  #define GameResWidth 300
  #define GameResHeight 300
  #define GameBpp 32
  #define GAME_MEMORY_SIZE (GameResWidth*GameResHeight * (GameBpp/8))
  #define color_black 0,0,0
  #define color_red 255,0,0
  #define color_blue 0,0,255
  #define color_green 0,255,0
  #define color_white 255,255,255
  #define swap(a,b){int temp=a;a=b;b=temp;}
  #define main() WINAPI WinMain(HINSTANCE Instance, HINSTANCE PreviousInstance, LPSTR CommandLine, int CmdShow)
//
//function declerations
    void fill_screen(BYTE , BYTE , BYTE );
    void set_point(int , int , BYTE , BYTE , BYTE );
    void Render(void);
    void checkWindowSize(void);
    void draw_line(int , int , int , int, byte,byte,byte);
    void draw_triangle(int,int,int,int,int,int,byte,byte,byte);
    void loadobj();
    void drawobj();
    void sort_obj();
    void rotate_obj();
//
// structures
  typedef struct {
    float x , y , z;
  }Vertex;
  typedef struct {
    float x , y , z;
  }Normal;
  typedef struct{
    int v1 , v2 , v3;
    int nx , ny , nz;
  }Triangle;
  typedef struct{
    float x1,y1,z1;
    float x2,y2,z2;
    float x3,y3,z3;
    float nx3d,ny3d,nz3d;
  }Triangle3d;
  typedef struct GameBitmap{
    BITMAPINFO BitmapInfo;
    void *memory;
  }GameBitmap;
//

// Global Variables
    BOOL run = true;
    HWND gGameWindow;
    RECT windowRect;
    int windowWidth = 640;
    int windowHeight = 480;
    BOOL GameStart = 1;
    MSG Message = {0};
    GameBitmap gDrawingSurface;
    Triangle3d* triangle3d_buffer = NULL;
    Triangle3d* sort3d_buffer = NULL;
    Vertex* vertex_buffer = NULL;
    Triangle* triangle_buffer = NULL;
    Normal* normal_buffer = NULL;
    int vcount = 0 , tcount = 0 , ncount = 0;
//

LRESULT CALLBACK MainWindowProc(HWND WindowHandle,UINT Message,WPARAM wParam,LPARAM lParam){ 
 
    switch (Message) 
    { 
        case WM_CREATE: 
            return 0; 
        case WM_SIZE: 
            checkWindowSize();
            return 0;
        case WM_DESTROY: 
            run = FALSE;
            PostQuitMessage(0); 
            return 0; 
        default: 
            return DefWindowProc(WindowHandle, Message, wParam, lParam); 
    } 
    return 0; 
    }

DWORD start(void){

    WNDCLASSEXA WindowClass = {0};
    WindowClass.cbSize   = sizeof(WNDCLASSEXA);
    WindowClass.lpfnWndProc   = MainWindowProc;
    WindowClass.hInstance     = GetModuleHandleA(NULL);
    WindowClass.lpszClassName = "OBJ_Loader";
    WindowClass.hbrBackground = CreateSolidBrush(RGB(45,0,0));
    
    RegisterClassExA(&WindowClass);

     gDrawingSurface.BitmapInfo.bmiHeader.biSize = sizeof(gDrawingSurface.BitmapInfo.bmiHeader);
     gDrawingSurface.BitmapInfo.bmiHeader.biWidth = GameResWidth;
     gDrawingSurface.BitmapInfo.bmiHeader.biHeight = -GameResHeight;
     gDrawingSurface.BitmapInfo.bmiHeader.biBitCount = GameBpp;
     gDrawingSurface.BitmapInfo.bmiHeader.biCompression = BI_RGB;
     gDrawingSurface.BitmapInfo.bmiHeader.biPlanes = 1;
     gDrawingSurface.memory = VirtualAlloc(NULL,GAME_MEMORY_SIZE,MEM_RESERVE|MEM_COMMIT , PAGE_READWRITE );
     memset(gDrawingSurface.memory, 0x00, GAME_MEMORY_SIZE); 
     gGameWindow   = CreateWindowExA(WS_EX_CLIENTEDGE,WindowClass.lpszClassName,"OBJ_LOADER",WS_OVERLAPPEDWINDOW|WS_VISIBLE,
     CW_USEDEFAULT, CW_USEDEFAULT,640,480,NULL,NULL,GetModuleHandleA(NULL), NULL);
    if (gGameWindow == NULL)
    {
    return (0);
    }
    
    }

void Events(void){
    while (PeekMessageA(&Message, NULL, 0, 0, PM_REMOVE))
    {
        DispatchMessageA(&Message);
    }
  }


void Render(void){
   if (GameStart ==1){
    checkWindowSize();
    GameStart = 0;
   }
    HDC DeviceContext = GetDC(gGameWindow);

    StretchDIBits(DeviceContext, 0, 0, windowWidth, windowHeight, 0, 0, GameResWidth, GameResHeight,
                  gDrawingSurface.memory, &gDrawingSurface.BitmapInfo, DIB_RGB_COLORS, SRCCOPY);

    // Release device context
    ReleaseDC(gGameWindow, DeviceContext);
    Sleep(1);
    }


void fill_screen(BYTE red, BYTE green, BYTE blue){
    // Fill the memory with the desired color
    for (int y = 0; y < GameResHeight; ++y)
    {
        for (int x = 0; x < GameResWidth; ++x)
        {
            // Calculate the index for the pixel in the memory buffer
            int pixelIndex = (y * GameResWidth + x) * 4; // 4 bytes per pixel for 32 BPP
            BYTE *pixel = (BYTE *)gDrawingSurface.memory + pixelIndex;

            // Set the pixel color
            pixel[0] = blue;  // Blue component
            pixel[1] = green; // Green component
            pixel[2] = red;   // Red component
            pixel[3] = 0;     // Alpha component (not used)
        }
    }
    }

void set_point(int x , int y, BYTE red, BYTE green, BYTE blue){
    if(x <GameResWidth && y < GameResHeight){
            int pixelIndex = (y * GameResWidth + x) * 4;
            BYTE *pixel = (BYTE *)gDrawingSurface.memory + pixelIndex;
            pixel[0] = blue;  // Blue component
            pixel[1] = green; // Green component
            pixel[2] = red;   // Red component
            pixel[3] = 0;     // Alpha component (not used)
    }
    }

void checkWindowSize(){
 GetClientRect(gGameWindow, &windowRect);
 windowWidth = windowRect.right - windowRect.left;
 windowHeight = windowRect.bottom - windowRect.top;
  }
void openfile(){
    FILE* file = fopen("cube.obj","r");
    char line[128];
    while(fgets(line,sizeof(line),file)){
        if(line[0] == 'v' && line[1]== ' ') vcount++;
        if(line[0] == 'f') tcount++;
        if(line[0] == 'v' && line[1]== 'n') ncount++;
    }
    triangle_buffer = malloc(sizeof(Triangle)*tcount);
    vertex_buffer = malloc(sizeof(Vertex)*vcount);
    normal_buffer = malloc(sizeof(Normal)*ncount);
    fclose(file);
    file = fopen("cube.obj","r");
    int vcounter = 0,tcounter = 0,ncounter = 0;
    while(fgets(line,sizeof(line),file)){
          if(line[0] == 'v'&& line[1]==' '){
            sscanf(line,"v %f %f %f",&vertex_buffer[vcounter].x ,&vertex_buffer[vcounter].y,&vertex_buffer[vcounter].z);
            vcounter++;
          }
          if(line[0] == 'v'&& line[1]=='n'){
            sscanf(line,"vn %f %f %f",&normal_buffer[ncounter].x ,&normal_buffer[ncounter].y,&normal_buffer[ncounter].z);
            ncounter++;
          }
          if(line[0] == 'f'){
             sscanf(line,"f %d//%d %d//%d %d//%d",&triangle_buffer[tcounter].v1 , &triangle_buffer[tcounter].nx,
                    &triangle_buffer[tcounter].v2,&triangle_buffer[tcounter].ny,
                    &triangle_buffer[tcounter].v3,&triangle_buffer[tcounter].nz);
            
            tcounter++;
          }
    }
    fclose(file);
  }

void draw_line(int x1 , int y1 , int x2 , int y2 ,byte r,byte g, byte b){
  int sx = x2-x1;
  int sy = y2-y1;
  int dx = abs(sx);
  int dy = abs(sy);
  int k ; 
  int xn = x1;
  int yn = y1;
 
  if (dx>=dy)
   k = 2 * sy - sx;
  else
  k = 2 * sx - sy;
  int dg = (dx>dy)? dx:dy;
  for (int i = 0 ; i <dg ; i++)
  {
   if (k>0){
      if(dx>=dy){
         k = k+ 2*dy - 2*dx;
      }
      else{
         k = k + 2*dx - 2*dy;
      }
      if (sx < 0)
      xn = xn-1;
      else
      xn = xn+1;

      if(sy<0)
      yn = yn-1;
      else
      yn = yn+1;
      set_point(xn,yn,r,g,b);

   }
   else{
      if (dx>=dy)
      k = k+2*dy;
      else
      k = k + 2 *dx;
      if (dx>=dy){
         if (sx>=0)
            xn = xn+1;
         else
            xn = xn-1;
         }
         else{
            if (sy>=0)
            yn = yn+1;
            else
            yn = yn-1;
         }
         set_point(xn,yn,r,g,b);
      }

   }
  }
void draw_triangle (int x0,int y0,int x1,int y1,int x2 , int y2 , byte r , byte g ,byte b){
     if ((x0 == x1 && y0 == y1) || (x1 == x2 && y1 == y2) || (x0 == x2 && y0 == y2)) 
    {
        return;
    }
    if (x0 < 0 || x0 >= GameResWidth ||x1 < 0 || x1 >= GameResWidth ||x2 < 0 || x2 >= GameResWidth ||
      y0 < 0 || y0 >= GameResHeight ||y1 < 0 || y1 >= GameResHeight ||y2 < 0 || y2 >= GameResHeight) 
     {
    return;
     }
    int area = (x1 - x0) * (y2 - y0) - (x2 - x0) * (y1 - y0);
    if (area == 0) {
        return;
    }
    
    if(y0>y1){swap(x0,x1); swap(y0,y1);}
    if(y1>y2){swap(x1,x2); swap(y1,y2);}
    if(y0>y1){swap(x0,x1); swap(y0,y1);}

    
     int loop;
     int x = x0;
     int dx = x1-x0;
     int dy = y1-y0;
     int abs_dx = abs(dx);
     int abs_dy = abs(dy);
     int step;

     int lx = x0;
     int ldx = x2-x0;
     int ldy = y2-y0;
     int labs_dx = abs(ldx);
     int labs_dy = abs(ldy);
     int lstep;

     if(y0==y1){
        x = x0;
        dx = x2-x0;
        dy = y2-y0;
        abs_dx = abs(dx);
        abs_dy = abs(dy);


        lx = x1;
        ldx = x2-x1;
        ldy = y2-y1;
        labs_dx = abs(ldx);
        labs_dy = abs(ldy);


     }

      if (ldx>0)
        lstep = 1;
     else
        lstep = -1;


     
     if (dx>0)
        step = 1;
     else
        step = -1;
    int lerror = labs_dx/2;    
    int error = abs_dx/2;

    if(y0 != y1 && y1 != y2 && y0 != y2)
        loop = y1;
    else
        loop = y2;

    for (int y = y0; y <= loop; y++)
    {
        int min = x , max = lx;
        if(min>max){swap(min,max)};
        for(int d = min; d <= max; d++){
            set_point(d,y,r,g,b);
        }
        error = error + abs_dx;
        lerror = lerror + labs_dx;

        while(error>= abs_dy){
            x=x+step;
            error = error-abs_dy;
        }
         while(lerror>= labs_dy){
            lx=lx+lstep;
            lerror = lerror-labs_dy;
        }
    }
    if(y0 != y1 && y1 != y2 && y0 != y2){
     draw_triangle(x1,y1,lx,y1,x2, y2 ,r,g,b);
    }
    }


void loadobj(){
    openfile();
    triangle3d_buffer = malloc(sizeof(Triangle3d)* tcount);
    sort3d_buffer = malloc(sizeof(Triangle3d)* tcount);
    for (int i=0; i < tcount ; i++){
      Triangle x = triangle_buffer[i];
      Vertex vx1 = vertex_buffer[x.v1 - 1];
      Vertex vx2 = vertex_buffer[x.v2 - 1];
      Vertex vx3 = vertex_buffer[x.v3 - 1];
      Normal nn = normal_buffer[x.nx-1];

      triangle3d_buffer[i].x1 = vx1.x;
      triangle3d_buffer[i].y1 = vx1.y;
      triangle3d_buffer[i].z1 = vx1.z;
      triangle3d_buffer[i].nx3d = nn.x;


      triangle3d_buffer[i].x2 = vx2.x;
      triangle3d_buffer[i].y2 = vx2.y;
      triangle3d_buffer[i].z2 = vx2.z;
      triangle3d_buffer[i].ny3d = nn.y;

      
      triangle3d_buffer[i].x3 = vx3.x;
      triangle3d_buffer[i].y3 = vx3.y;
      triangle3d_buffer[i].z3 = vx3.z;
      triangle3d_buffer[i].nz3d = nn.z;}
      sort_obj();
    }
void sort_obj(){
  for (int i=0; i<tcount-1; i++){
      for(int j=i+1; j<tcount; j++){
        float sum1 = triangle3d_buffer[i].z1+triangle3d_buffer[i].z2+triangle3d_buffer[i].z3;
        float sum2 = triangle3d_buffer[j].z1+triangle3d_buffer[j].z2+triangle3d_buffer[j].z3;

        if(sum1>sum2){
          Triangle3d temp = triangle3d_buffer[i];
          triangle3d_buffer[i] = triangle3d_buffer[j];
          triangle3d_buffer[j] = temp;
        }
      }
    }
  }
void drawobj(){
   for (int i = 0; i < tcount; i++){

  float dp=(triangle3d_buffer[i].nx3d * 0.4718) + (triangle3d_buffer[i].ny3d * -0.6891) + (triangle3d_buffer[i].nz3d * 0.5500);
  int cp = (int)(((dp+1.0f)/2.0f)*255.0f);

      draw_triangle(
        triangle3d_buffer[i].x1,
        triangle3d_buffer[i].y1,
        triangle3d_buffer[i].x2,
        triangle3d_buffer[i].y2,
        triangle3d_buffer[i].x3,
        triangle3d_buffer[i].y3,
        cp,cp,cp
      );
    }}

void rotate_obj(){
  for (int i = 0; i < tcount; i++){
    sort3d_buffer[i] = triangle3d_buffer[i];
  }
  for (int i = 0; i < tcount; i++){
    sort3d_buffer[i].x1 = (triangle3d_buffer[i].x1 - 100.0f) * cos(0.0174533f) + triangle3d_buffer[i].z1 * sin(0.0174533f) + 100.0f;
    sort3d_buffer[i].z1 = - (triangle3d_buffer[i].x1 - 100.0f) * sin(0.0174533f) + triangle3d_buffer[i].z1 * cos(0.0174533f);
    sort3d_buffer[i].y1 = triangle3d_buffer[i].y1;

    // Rotate vertex 2
    sort3d_buffer[i].x2 = (triangle3d_buffer[i].x2 - 100.0f) * cos(0.0174533f) + triangle3d_buffer[i].z2 * sin(0.0174533f) + 100.0f;
    sort3d_buffer[i].z2 = - (triangle3d_buffer[i].x2 - 100.0f) * sin(0.0174533f) + triangle3d_buffer[i].z2 * cos(0.0174533f);
    sort3d_buffer[i].y2 = triangle3d_buffer[i].y2;

    // Rotate vertex 3
    sort3d_buffer[i].x3 = (triangle3d_buffer[i].x3 - 100.0f) * cos(0.0174533f) + triangle3d_buffer[i].z3 * sin(0.0174533f) + 100.0f;
    sort3d_buffer[i].z3 = - (triangle3d_buffer[i].x3 - 100.0f) * sin(0.0174533f) + triangle3d_buffer[i].z3 * cos(0.0174533f);
    sort3d_buffer[i].y3 = triangle3d_buffer[i].y3;

    // Rotate normal (no pivot)
    sort3d_buffer[i].nx3d = triangle3d_buffer[i].nx3d * cos(0.0174533f) + triangle3d_buffer[i].nz3d * sin(0.0174533f);
    sort3d_buffer[i].ny3d = triangle3d_buffer[i].ny3d;
    sort3d_buffer[i].nz3d = - triangle3d_buffer[i].nx3d * sin(0.0174533f) + triangle3d_buffer[i].nz3d * cos(0.0174533f);
  }

  for (int i=0; i<tcount-1; i++){
      for(int j=i+1; j<tcount; j++){
        float sum1 = sort3d_buffer[i].z1+sort3d_buffer[i].z2+sort3d_buffer[i].z3;
        float sum2 = sort3d_buffer[j].z1+sort3d_buffer[j].z2+sort3d_buffer[j].z3;

        if(sum1>sum2){
          Triangle3d temp = sort3d_buffer[i];
          sort3d_buffer[i] = sort3d_buffer[j];
          sort3d_buffer[j] = temp;
        }
      }
    }
   for (int i = 0; i < tcount; i++){
    triangle3d_buffer[i] = sort3d_buffer[i];
  }
   for (int i = 0; i < tcount; i++){
      float dp=(sort3d_buffer[i].nx3d * 0.4718) + (sort3d_buffer[i].ny3d * -0.6891) + (sort3d_buffer[i].nz3d * 0.5500);
      int cp = (int)(((dp+1.0f)/2.0f)*255.0f);

      draw_triangle(
        sort3d_buffer[i].x1,
        sort3d_buffer[i].y1,
        sort3d_buffer[i].x2,
        sort3d_buffer[i].y2,
        sort3d_buffer[i].x3,
        sort3d_buffer[i].y3,
        cp,cp,cp
      );
    }
    }
