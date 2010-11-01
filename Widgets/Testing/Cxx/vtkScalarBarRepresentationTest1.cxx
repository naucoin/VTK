#include "vtkScalarBarRepresentation.h"
#include <vtkScalarBarActor.h>
#include <stdlib.h>
#include <iostream>

#include "WidgetTestingMacros.h"

int vtkScalarBarRepresentationTest1(int , char * [] )
{
  vtkSmartPointer< vtkScalarBarRepresentation > node1 = vtkSmartPointer< vtkScalarBarRepresentation >::New();
  
  EXERCISE_BASIC_BORDER_REPRESENTATION_METHODS(vtkScalarBarRepresentation, node1); 
  EXERCISE_BASIC_PROP_METHODS(vtkScalarBarRepresentation, node1);

  // it's clamped
  TEST_SET_GET_INT(node1, Orientation, 1);
  TEST_SET_GET_INT(node1, Orientation, 1);

  
  node1->SetScalarBarActor(NULL);
  vtkSmartPointer<vtkScalarBarActor> b = node1->GetScalarBarActor();
  if (b != NULL)
    {
    std::cerr << "ERROR: expected a null scalar bar actor, got " << b << std::endl;
    return EXIT_FAILURE;
    }

  vtkSmartPointer<vtkScalarBarActor> a = vtkSmartPointer<vtkScalarBarActor>::New();
  node1->SetScalarBarActor(a);
  b = node1->GetScalarBarActor();
  if (b == NULL)
    {
    std::cerr << "ERROR: expected a non null scalar bar actor, got NULL\n";
    return EXIT_FAILURE;
    }
  
  return EXIT_SUCCESS;
}
