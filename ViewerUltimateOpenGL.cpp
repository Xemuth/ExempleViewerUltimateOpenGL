#include <CtrlLib/CtrlLib.h>
#include "VisualCamera.h"

using namespace Upp;

struct UltimateOpenGLExemple : GLCtrl{
	UltimateOpenGL_Context context;
	Ctrl* topWindow = nullptr;
	bool isLoaded = false;
	bool pressed = false;
	Point StartPress;
	
	virtual void GLPaint() {
		if(!isLoaded){
			context.Initialise(); //First thing to do

			Scene& sc = context.CreateScene("scene1");

			VisualCamera& cam3 = sc.CreateCamera<VisualCamera>("StaticVisualCamera");
			cam3.DisableRoll().SetMouvementSpeed(60.0f).SetFOV(45.0f).GetTransform().SetNewPosition(glm::vec3(0.0f,0.0f,15.0f));

			sc.GetSkyBox().SetColor(159,208,225);
			sc.CreateGameObject<Object3D>("Upp").LoadModel(TransformFilePath("\\obj upp\\upp.obj")).GetTransform().SetNewScale(glm::vec3(0.05f,0.05f,0.05f));
			cam3.SetObjectToFocus(sc.GetGameObject<Object3D>("Upp"));
			sc.Load();
			isLoaded=true;
		}
		context.Draw();
		Refresh();
	}
	virtual void GLResize(int w, int h) {
		glViewport(0, 0, (GLsizei)w, (GLsizei)h);
		context.SetScreenSize(h,w);
		HSizePos(1, 1).VSizePos(1, 1);
	}
	virtual bool Key(dword key,int count){
		if( key == K_Z){
			//Exemple of key press
			//context.GetActiveScene().GetActiveCamera().ProcessKeyboardMouvement(CM_FORWARD);
			return true;
		}
		if( key == K_ESCAPE){
			topWindow->Close();
		}
		return false;
	}
	virtual void MouseMove(Point p, dword) {
		if(isLoaded){
			if(pressed){
				context.GetActiveScene().GetActiveCamera().ProcessMouveMouvement(p.x - StartPress.x,p.y - StartPress.y);
				StartPress = p;
			}
		}
	}
	
	virtual void LeftDown(Point p, dword){
		StartPress = p;
		ShowCursor(false);
		pressed = true;
		return;
	}
	
	virtual void LeftUp(Point p, dword){
		ShowCursor(true);
		pressed = false;
		return;
	}
	
	virtual void MouseLeave(){
		ShowCursor(true);
		pressed = false;
		return;
	}
	
	virtual void MouseWheel(Point p,int zdelta,dword keyflags){ //mouse wheel handling
		if(zdelta > 0){
			context.GetActiveScene().GetActiveCamera().ProcessMouseScroll(1);
		}else if(zdelta< 0){
			context.GetActiveScene().GetActiveCamera().ProcessMouseScroll(-1);
		}
	}
	UltimateOpenGL_Context& GetContext(){
		return context;
	}
	
	void SetParent(Ctrl* ptr){
		topWindow = ptr;
	}
	
	String TransformFilePath(String FilePath){
		String FilePathBuffer = String(__FILE__);
		String FilePathTempory ="";
		FilePathBuffer = Replace(FilePathBuffer,Vector<String>{"\\"},Vector<String>{"/"});
		
		Replace(FilePath,Vector<String>{"\\"},Vector<String>{"/"});
		FilePathTempory  = FilePathBuffer.Left(FilePathBuffer.ReverseFind("/")) + FilePath;
		if(!FileExists(FilePathTempory)){
			return (GetExeFolder()  +FilePath);
		}
		return FilePathTempory;
	}
};

#define LAYOUTFILE <ViewerUltimateOpenGL/TestWidgetSize.lay>
#include <CtrlCore/lay.h>

class TestWidgetSize : public WithExempleWindowUltimateOpenGL<TopWindow> {
public:
	typedef TestWidgetSize CLASSNAME;

	TestWidgetSize(){
		CtrlLayout(*this, "Viewer UltimateOpenGL");
		SetRect(0, 0, 800, 600);
		glOne.SetParent(this);
	}
	
	virtual bool Key(dword key,int count){
		glOne.Key(key,count);
		return false;
	}
	
	virtual void Layout(){
		glOne.GLResize(GetSize().cx,GetSize().cy);
	}
};


GUI_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	Ctrl::GlobalBackPaint();
	TestWidgetSize() .Sizeable().Zoomable().Run();
}
