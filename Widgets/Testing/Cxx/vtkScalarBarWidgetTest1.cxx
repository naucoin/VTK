#include "vtkScalarBarWidget.h"
#include "vtkScalarBarRepresentation.h"
#include "vtkScalarBarActor.h"

#include <stdlib.h>
#include <iostream>

#include "WidgetTestingMacros.h"

int vtkScalarBarWidgetTest1(int , char * [] )
{
  vtkSmartPointer< vtkScalarBarWidget > node1 = vtkSmartPointer< vtkScalarBarWidget >::New();

  EXERCISE_BASIC_BORDER_METHODS (node1 );
  
  vtkSmartPointer<vtkScalarBarRepresentation> rep1 = vtkSmartPointer<vtkScalarBarRepresentation>::New();
  node1->SetRepresentation(rep1);

  vtkSmartPointer<vtkScalarBarRepresentation> rep2 = node1->GetScalarBarRepresentation();
  
  vtkScalarBarActor *textActor = node1->GetScalarBarActor();
  if (textActor)
    {
    std::cout << "ScalarBar actor is not null" << std::endl;
    }
  else
    {
    std::cout << "ScalarBar actor is null" << std::endl;
    }
  vtkSmartPointer<vtkScalarBarActor> textActor2 = vtkSmartPointer<vtkScalarBarActor>::New();
  node1->SetScalarBarActor(textActor2);
  if (node1->GetScalarBarActor() != textActor2)
    {
    std::cerr << "Failed to get back expected text actor" << std::endl;
    return EXIT_FAILURE;
    }

  TEST_SET_GET_BOOLEAN(node1, Repositionable);
  
  return EXIT_SUCCESS;
}
