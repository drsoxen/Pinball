#define SAFEDELETE(x) { if(x){ delete x;x = 0; } }
#define ReleaseCOM(x) { if(x){ x->Release();x = 0; } }

#define DEG_TO_RAD(x)  x * 0.0174532925
#define RAD(x) x * (180.0f / 3.1415926535f)

#define TWOPI 6.283185307179586476925286766559f
#define PI 3.1415926535897932384626433832795f
#define HALFPI 1.5707963267948966192313216916398f

#define LogMessage(x) DevConsole::GetInstance()->logMessage(x);
#define LogError(x) DevConsole::GetInstance()->logError(x);
#define LogFormattedMessage(x) DevConsole::GetInstance()->logFormattedMessage(x);

#define Input InputManager::GetInstance()
