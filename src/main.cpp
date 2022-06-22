#include "geometrycentral/surface/geometry.h"

#include "geometrycentral/surface/surface_mesh.h"
#include "geometrycentral/surface/manifold_surface_mesh.h"
#include "geometrycentral/surface/meshio.h"

#include "geometrycentral/surface/detect_symmetry.h"

#include "polyscope/polyscope.h"
#include "polyscope/surface_mesh.h"

#include "args/args.hxx"
#include "imgui.h"


using namespace geometrycentral;
using namespace geometrycentral::surface;

// == Geometry-central data
// std::unique_ptr<ManifoldSurfaceMesh> mesh;
// std::unique_ptr<VertexPositionGeometry> geometry;
// VertexPositionGeometry geometry;

// std::unique_ptr<SurfaceMesh> mesh;


// Polyscope visualization handle, to quickly add data to the surface
// polyscope::SurfaceMesh *psMesh;

// Some algorithm parameters
float param1 = 42.0;

// Example computation function -- this one computes and registers a scalar
// quantity
void doWork()
{
    polyscope::warning("Computing Symmetry.\nalso, parameter value = " +
                       std::to_string(param1));    

    // SymmetryResult symmetryResult = detectSymmetryAuto(geometry);
}

// A user-defined callback, for creating control panels (etc)
// Use ImGUI commands to build whatever you want here, see
// https://github.com/ocornut/imgui/blob/master/imgui.h
void myCallback()
{

    if (ImGui::Button("do work"))
    {
        doWork();
    }

    ImGui::SliderFloat("param", &param1, 0., 100.);
}

int main(int argc, char **argv)
{

    // Configure the argument parser
    args::ArgumentParser parser("geometry-central & Polyscope example project");
    args::Positional<std::string> inputFilename(parser, "mesh", "A mesh file.");

    // Parse args
    try
    {
        parser.ParseCLI(argc, argv);
    }
    catch (args::Help &h)
    {
        std::cout << parser;
        return 0;
    }
    catch (args::ParseError &e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    // Make sure a mesh name was given
    if (!inputFilename)
    {
        std::cerr << "Please specify a mesh file as argument" << std::endl;
        return EXIT_FAILURE;
    }

    // Initialize polyscope
    polyscope::init();

    // Set the callback function
    polyscope::state::userCallback = myCallback;

    // Load mesh
    std::unique_ptr<SurfaceMesh> mesh;
    std::unique_ptr<VertexPositionGeometry> geometry;
    std::tie(mesh, geometry) = readManifoldSurfaceMesh(args::get(inputFilename));

    // std::unique_ptr<VertexPositionGeometry> mesh;
    std::unique_ptr<SurfaceMesh> mesh2;
    // VertexPositionGeometry vertexPositionGeometry = VertexPositionGeometry(mesh2);
    SymmetryResult symmetryResult = detectSymmetryAuto(*mesh, *geometry);

    // add the mesh to the gui
    polyscope::registerSurfaceMesh("my mesh", 
        geometry->vertexPositions, mesh->getFaceVertexList());

    // Give control to the polyscope gui
    polyscope::show();

    return EXIT_SUCCESS;
}
