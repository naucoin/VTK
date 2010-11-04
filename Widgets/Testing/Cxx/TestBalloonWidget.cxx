/*=========================================================================

  Program:   Visualization Toolkit
  Module:    TestBalloonWidget.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//
// This example tests the vtkHoverWidget and vtkBalloonWidget.

// First include the required header files for the VTK classes we are using.
#include "vtkSmartPointer.h"
#include "vtkBalloonWidget.h"
#include "vtkBalloonRepresentation.h"
#include "vtkSphereSource.h"
#include "vtkCylinderSource.h"
#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkCommand.h"
#include "vtkInteractorEventRecorder.h"
#include "vtkRegressionTestImage.h"
#include "vtkDebugLeaks.h"
#include "vtkTestUtilities.h"
#include "vtkTIFFReader.h"
#include "vtkCellPicker.h"
#include "vtkPropPicker.h"
#include "vtkPolyData.h"
#include "vtkPointData.h"
#include "vtkCellArray.h"
#include "vtkPoints.h"
#include "vtkFloatArray.h"
#include <sstream>

class vtkBalloonCallback : public vtkCommand
{
public:
  static vtkBalloonCallback *New() 
    { return new vtkBalloonCallback; }
  virtual void Execute(vtkObject *caller, unsigned long event, void*)
    {
      // std::cout << "Event = " << event << std::endl;
      vtkBalloonWidget * balloonWidget = reinterpret_cast<vtkBalloonWidget*>(caller);
      vtkSmartPointer<vtkProp> currentProp = balloonWidget->GetCurrentProp();
      if (event == vtkCommand::TimerEvent && currentProp != NULL)
        { 
        //std::cout << "TimerEvent\n";
        if (balloonWidget->GetPicker() != NULL)
          {
          vtkSmartPointer<vtkCellPicker> cellPicker = vtkCellPicker::SafeDownCast(balloonWidget->GetPicker());
          int lastPointID = cellPicker->GetPointId();
          if (cellPicker != NULL &&
              lastPointID != -1)
            {
            //std::cout << "Have a cell picker, last picked vertex = " << lastPointID << std:: endl;
            //const char *currentString = balloonWidget->GetBalloonString(currentProp);
            //std::cout << "\tcurrent string = " << currentString << std::endl;
            std::stringstream ss;
            ss << "Picked point: id = ";
            ss << lastPointID;
            vtkSmartPointer<vtkActor> actor = vtkActor::SafeDownCast(currentProp);
            if (actor)
              {
              vtkSmartPointer<vtkMapper> mapper = actor->GetMapper();
              if (mapper && mapper->GetInput())
                {
                vtkSmartPointer<vtkPolyData> pd = vtkPolyData::SafeDownCast(mapper->GetInput());
                if (pd)
                  {
                  //std::cout << "\tHave poly data!\n";
                  if (pd->GetPointData() && pd->GetPointData()->GetNumberOfArrays() > 0)
                    {
                    //std::cout << "\thave scalars!\n";
                    int arrayIndex;
                    vtkSmartPointer<vtkDataArray> da = pd->GetPointData()->GetArray("scalars", arrayIndex);
                    if (da)
                      {
                      //std::cout<<"\thave data array\n";
                      vtkSmartPointer<vtkFloatArray> fa = vtkFloatArray::SafeDownCast(da);
                      if (fa)
                        {
                        float val = fa->GetValue(lastPointID);
                        if (val)
                          {
                          ss << ", scalar value = ";
                          ss << val;
                          }
                        }
                      }
                    }
                  }
                }
              }
              
			std::string newString = ss.str();
            std::cout << "\tPick string = " << newString.c_str() << std::endl;
            vtkBalloonRepresentation *rep = reinterpret_cast<vtkBalloonRepresentation*>(balloonWidget->GetRepresentation());
            if (0 && rep)
              {
              rep->SetBalloonText(newString.c_str());
              rep->BuildRepresentation();
              }
            else
              {
              std::cout << "\tremoving and adding balloon\n";
              balloonWidget->RemoveBalloon(currentProp);
              balloonWidget->AddBalloon(currentProp, newString.c_str(), NULL);
              }
            }
          }        
        else
          {
          std::cerr << "No picker\n";
          }
        }
    }
};

int TestBalloonWidget( int argc, char *argv[] )
{
  // Create the RenderWindow, Renderer and both Actors
  //
  vtkSmartPointer<vtkRenderer> ren1 = vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
  renWin->AddRenderer(ren1);

  vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  iren->SetRenderWindow(renWin);

  // Create a test pipeline
  //
  vtkSmartPointer<vtkSphereSource> ss = vtkSmartPointer<vtkSphereSource>::New();
  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInput(ss->GetOutput());
  vtkSmartPointer<vtkActor> sph = vtkSmartPointer<vtkActor>::New();
  sph->SetMapper(mapper);

  vtkSmartPointer<vtkCylinderSource> cs = vtkSmartPointer<vtkCylinderSource>::New();
  vtkSmartPointer<vtkPolyDataMapper> csMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  csMapper->SetInput(cs->GetOutput());
  vtkSmartPointer<vtkActor> cyl = vtkSmartPointer<vtkActor>::New();
  cyl->SetMapper(csMapper);
  cyl->AddPosition(5,0,0);
  
  vtkSmartPointer<vtkConeSource> coneSource = vtkSmartPointer<vtkConeSource>::New();
  vtkSmartPointer<vtkPolyDataMapper> coneMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  coneMapper->SetInput(coneSource->GetOutput());
  vtkSmartPointer<vtkActor> cone = vtkSmartPointer<vtkActor>::New();
  cone->SetMapper(coneMapper);
  cone->AddPosition(0,5,0);

  // create a diamond glyph
  vtkSmartPointer<vtkPoints> diamondGlyphPoints = vtkSmartPointer<vtkPoints>::New();
  diamondGlyphPoints->SetNumberOfPoints(6);
  diamondGlyphPoints->InsertPoint(0, 0.5, 0, 0);
  diamondGlyphPoints->InsertPoint(1, 0, 0.5, 0);
  diamondGlyphPoints->InsertPoint(2, 0, 0, 0.5);
  diamondGlyphPoints->InsertPoint(3, -0.5, 0, 0);
  diamondGlyphPoints->InsertPoint(4, 0, -0.5, 0);
  diamondGlyphPoints->InsertPoint(5, 0, 0, -0.5);

  vtkSmartPointer<vtkCellArray>diamondGlyphPolys = vtkSmartPointer<vtkCellArray>::New();
  diamondGlyphPolys->InsertNextCell( 4 );
  diamondGlyphPolys->InsertCellPoint(0);
  diamondGlyphPolys->InsertCellPoint(1);
  diamondGlyphPolys->InsertCellPoint(3);
  diamondGlyphPolys->InsertCellPoint(4);
  
  diamondGlyphPolys->InsertNextCell(4);
  diamondGlyphPolys->InsertCellPoint(1);
  diamondGlyphPolys->InsertCellPoint(2);
  diamondGlyphPolys->InsertCellPoint(4);
  diamondGlyphPolys->InsertCellPoint(5);

  diamondGlyphPolys->InsertNextCell(4);
  diamondGlyphPolys->InsertCellPoint(2);
  diamondGlyphPolys->InsertCellPoint(0);
  diamondGlyphPolys->InsertCellPoint(5);
  diamondGlyphPolys->InsertCellPoint(3);

  vtkSmartPointer<vtkCellArray> diamondGlyphLines = vtkSmartPointer<vtkCellArray>::New(); 
          
  diamondGlyphLines->InsertNextCell(2);
  diamondGlyphLines->InsertCellPoint(0);
  diamondGlyphLines->InsertCellPoint(3);

  diamondGlyphLines->InsertNextCell(2);
  diamondGlyphLines->InsertCellPoint(1);
  diamondGlyphLines->InsertCellPoint(4);

  diamondGlyphLines->InsertNextCell(2);                                         
  diamondGlyphLines->InsertCellPoint(2);
  diamondGlyphLines->InsertCellPoint(5);
  vtkSmartPointer<vtkPolyData> DiamondGlyphPolyData = vtkSmartPointer<vtkPolyData>::New();
  DiamondGlyphPolyData->SetPoints(diamondGlyphPoints);
  diamondGlyphPoints->Delete();
  DiamondGlyphPolyData->SetPolys(diamondGlyphPolys);
  DiamondGlyphPolyData->SetLines(diamondGlyphLines);
  diamondGlyphPolys->Delete();
  diamondGlyphLines->Delete();

  vtkSmartPointer<vtkPolyDataMapper> diamondMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  diamondMapper->SetInput(DiamondGlyphPolyData);
  vtkSmartPointer<vtkActor> diamondActor = vtkSmartPointer<vtkActor>::New();
  diamondActor->SetMapper(diamondMapper);
  diamondActor->AddPosition(5,5,0);
  
  // add some scalars to the cone
  vtkPolyData *pd = vtkPolyData::SafeDownCast(DiamondGlyphPolyData);
  if (pd && pd->GetPointData())
    {
    vtkIdType numVerts = pd->GetNumberOfPoints();
    vtkSmartPointer<vtkFloatArray> scalarArray = vtkSmartPointer<vtkFloatArray>::New();
    scalarArray->SetName("scalars");
    for (int i = 0; i < numVerts; i++)
      {
      scalarArray->InsertNextTuple1(i * 2.0);
      }
    pd->GetPointData()->SetScalars(scalarArray);
    
    }
  // Create the widget
  vtkSmartPointer<vtkBalloonRepresentation> rep = vtkSmartPointer<vtkBalloonRepresentation>::New();
  rep->SetBalloonLayoutToImageRight();

  vtkSmartPointer<vtkBalloonWidget> widget = vtkSmartPointer<vtkBalloonWidget>::New();
  widget->SetInteractor(iren);
  widget->SetRepresentation(rep);
  vtkSmartPointer<vtkCellPicker> cellPicker = vtkSmartPointer<vtkCellPicker>::New();
  widget->SetPicker(cellPicker);
  widget->AddBalloon(sph,"This is a sphere",NULL);
  widget->AddBalloon(cyl,"This is a cylinder",NULL);
  widget->AddBalloon(cone,"This is a cone, a really big cone, you wouldn't believe how big",NULL);
  widget->AddBalloon(diamondActor, "This is a diamond", NULL);
  vtkSmartPointer<vtkBalloonCallback> cbk = vtkSmartPointer<vtkBalloonCallback>::New();
  widget->AddObserver(vtkCommand::WidgetActivateEvent,cbk);
  widget->AddObserver(vtkCommand::TimerEvent, cbk);
  // Add the actors to the renderer, set the background and size
  //
  ren1->AddActor(sph);
  ren1->AddActor(cyl);
  ren1->AddActor(cone);
  ren1->AddActor(diamondActor);
  ren1->SetBackground(0.1, 0.2, 0.4);
  renWin->SetSize(300, 300);

  // record events
  vtkSmartPointer<vtkInteractorEventRecorder> recorder = vtkSmartPointer<vtkInteractorEventRecorder>::New();
  recorder->SetInteractor(iren);
  recorder->SetFileName("c:/record.log");
//  recorder->Record();
//  recorder->ReadFromInputStringOn();
//  recorder->SetInputString(eventLog);

  // render the image
  //
  iren->Initialize();
  renWin->Render();
  widget->On();
//  recorder->Play();

  // Remove the observers so we can go interactive. Without this the "-I"
  // testing option fails.
  recorder->Off();

  int retVal = vtkRegressionTestImage( renWin );
  if ( retVal == vtkRegressionTester::DO_INTERACTOR)
    {
    iren->Start();
    }

  return !retVal;

}


