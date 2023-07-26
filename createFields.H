Info<< "Reading field p\n" << endl;
volScalarField p
(
    IOobject
    (
        "p",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Reading field U\n" << endl;
volVectorField U
(
    IOobject
    (
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

/**************************** Design variables *********************************/
Info<< "Reading the design variables alpha \n" << endl;
volScalarField alpha_des
(
    IOobject
    (
        "alpha_des",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

/**************************** Material model **********************************/

Info << "\nSetting the material model:\n"; 

// Create dictionary of material model properties from I/O (Input/Ouput)
IOdictionary materialmodelProperties (
	IOobject (
		"materialmodelProperties",
		runTime.constant(),
		mesh,
		IOobject::MUST_READ_IF_MODIFIED,
		IOobject::NO_WRITE
	)
);

// q_penalization
dimensionedScalar q_penalization (
	"q_penalization",
	dimless, // dimensionSet(0, 0, 0, 0, 0, 0, 0),
	materialmodelProperties
);
// printDimensionedScalarInformation(q_penalization);

// κ_max
dimensionedScalar k_max (
	"k_max",
	dimensionSet(1, -3, -1, 0, 0, 0, 0),
	materialmodelProperties
);
// printDimensionedScalarInformation(k_max);

// κ_min
dimensionedScalar k_min (
	"k_min",
	dimensionSet(1, -3, -1, 0, 0, 0, 0),
	materialmodelProperties
);
// printDimensionedScalarInformation(k_min);

// ρ
dimensionedScalar rho_density (
	"rho_density",
	dimensionSet(1, -3, 0, 0, 0, 0, 0),
	materialmodelProperties
);
// printDimensionedScalarInformation(rho_density);

Info << "\n"; 

/**************************** original code **********************************/

#include "createPhi.H"

pressureReference pressureReference(p, simple.dict());

mesh.schemes().setFluxRequired(p.name());

autoPtr<viscosityModel> viscosity(viscosityModel::New(mesh));

autoPtr<incompressible::momentumTransportModel> turbulence
(
    incompressible::momentumTransportModel::New(U, phi, viscosity)
);

#include "createMRF.H"
#include "createFvModels.H"
#include "createFvConstraints.H"
