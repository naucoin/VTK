#include "vtkAngleRepresentation2D.h"

#include <stdlib.h>
#include <iostream>

#include "WidgetTestingMacros.h"

int vtkAngleRepresentation2DTest1(int , char * [] )
{
  vtkSmartPointer< vtkAngleRepresentation2D > node1 = vtkSmartPointer< vtkAngleRepresentation2D >::New();

  EXERCISE_BASIC_ANGLE_REPRESENTATION_METHODS(vtkAngleRepresentation2D, node1); 

  vtkLeaderActor2D *actor = node1->GetRay1();
  if (actor == NULL)
    {
    std::cout << "Ray 1 is null." << std::endl;
    }
  actor = node1->GetRay2();
  if (actor == NULL)
    {
    std::cout << "Ray 2 is null." << std::endl;
    }
  actor = node1->GetArc();
  if (actor == NULL)
    {
    std::cout << "Arc is null." << std::endl;
    }
  // test the angle value
  double pos[2] = {0.0, 0.0};
  node1->SetCenterDisplayPosition(pos);
//  node1->SetCenterWorldPosition(pos);
  pos[0] = 2.0;
//  node1->SetPoint1WorldPosition(pos);
  node1->SetPoint1DisplayPosition(pos);
  pos[0] = 0.0;
  pos[1] = 2.6;
  //node1->SetPoint2WorldPosition(pos);
  node1->SetPoint2DisplayPosition(pos);
  float deg = vtkMath::DegreesFromRadians(node1->GetAngle());
  std::cout << "GetAngle = " << node1->GetAngle() << ", degrees = " << deg << std::endl;
  if (deg != 90.0)
  {
    std::cerr << "ERROR: expected 90 degrees after setting points and center, got " << deg << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
