#pragma once
#include <fmt/format.h>
#include <optional>
#include <tuple>
#include <vector>

namespace Tarjan
{

class TarjanConnectivityAlgo
{
public:
    TarjanConnectivityAlgo( const std::vector<std::vector<size_t>> & adjacency_list_arg )
            : scc_list( std::vector<std::vector<size_t>>( 0 ) ),
              adjacency_list( adjacency_list_arg ),
              num_nodes( adjacency_list.size() ),
              num( std::vector<size_t>( num_nodes ) ),
              lowest( std::vector<size_t>( num_nodes ) ),
              visited( std::vector<bool>( num_nodes, false ) ),
              processed( std::vector<bool>( num_nodes, false ) ),
              stack( std::vector<size_t>( 0 ) ),
              index_counter( 0 )
    {
        run(); // Tarjan's algorithm
    }

    std::vector<std::vector<size_t>>
        scc_list; // Each element is a vector of indices corresponding to a strongly connected component (SCC)

private:
    std::vector<std::vector<size_t>> adjacency_list;
    int num_nodes;
    std::vector<size_t> num;     // holding vertex numbers
    std::vector<size_t> lowest;  // lowest[v] : minimum number of a vertex reachable from v
    std::vector<bool> visited;   // visited so DFS has seen these vertices (not necessarily processed)
    std::vector<bool> processed; // vertices which have been processed by DFS
    std::vector<size_t>
        stack; // stack of vertices to keep a working set of vertices. Holds all vertices reachable from the starting vertex
    size_t index_counter; // depth-first search node number counter

    // Depth-first search
    // v: Current vertex
    void depth_first_search( std::size_t v )
    {
        std::vector<size_t> scc;

        // Set things for the current vertex v
        num[v]    = index_counter;
        lowest[v] = num[v];
        index_counter += 1;
        visited[v] = true;
        stack.push_back( v );

        // Loop through neighbours of v
        // u is the neighbouring vertex
        for( auto & u : adjacency_list[v] )
        {
            // If u hasn't been visited do a DFS
            if( !visited[u] )
            {
                // recursive call to DFS
                depth_first_search( u );
                lowest[v] = std::min( lowest[v], lowest[u] );
            } // u not visited
            else
            {
                // If u has been processed, then it is a cross-edge and should be ignored.
                // If u has been visited but not processed:
                if( !processed[u] )
                {
                    lowest[v] = std::min( lowest[v], num[u] );
                } // u not processed
            }     // u has been visited
        }

        // Now v has been processed
        processed[v] = true;

        // Handle SCC if found
        if( lowest[v] == num[v] )
        {
            scc.resize( 0 );
            size_t scc_vertex;
            // Pop the stack
            scc_vertex = stack.back();
            stack.pop_back();
            while( scc_vertex != v )
            {
                scc.push_back( scc_vertex ); // Add to the SCC
                // Pop the stack
                scc_vertex = stack.back();
                stack.pop_back();
            } // unravelling stack
            // Add the last vertex
            scc.push_back( scc_vertex );
            // Now that we have found the SCC, add it
            // to the SCC list
            scc_list.push_back( scc );
        } // SCC found
    }

    // Actually run Tarjan's algorithm
    // for finding strongly connected components (SCCs)
    void run()
    {
        // Tarjan's algorithm takes the form of a series of DFS invocations
        for( size_t i_node = 0; i_node < num_nodes; ++i_node )
        {
            // Start from a node that has not been visited
            if( !visited[i_node] )
            {
                // Call the depth first search
                depth_first_search( i_node );
            }
        }
    }
};

// Run the test
void run()
{
    // Create the vector of vectors containing the indices
    // Example from:
    // https://www.baeldung.com/cs/scc-tarjans-algorithm
    std::vector<std::vector<size_t>> adjacency_list;
    auto buffer = std::vector<size_t>(); // contains indices
    // // --- Create the list
    buffer.resize( 1, 1 );
    adjacency_list.push_back( buffer ); // A or 0
    buffer.resize( 2 );
    buffer[0] = 2;
    buffer[1] = 3;
    adjacency_list.push_back( buffer ); // B or 1
    buffer.resize( 1 );
    buffer[0] = 0;
    adjacency_list.push_back( buffer ); // C or 2
    buffer[0] = 4;
    adjacency_list.push_back( buffer ); // D or 3
    buffer[0] = 5;
    adjacency_list.push_back( buffer ); // E or 4
    buffer[0] = 4;
    adjacency_list.push_back( buffer ); // F or 5
    // Second tree (G,H,I,J)
    buffer.resize( 2 );
    buffer[0] = 4;
    buffer[1] = 7;
    adjacency_list.push_back( buffer ); // G or 6
    buffer[0] = 5;
    buffer[1] = 8;
    adjacency_list.push_back( buffer ); // H or 7
    buffer.resize( 1 );
    buffer[0] = 9;
    adjacency_list.push_back( buffer ); // I or 8
    buffer.resize( 2 );
    buffer[0] = 6;
    buffer[1] = 7;
    adjacency_list.push_back( buffer ); // J or 9
    fmt::print( "Adjacency list:{}\n\n", adjacency_list );
    // ---

    auto tarjan_scc = TarjanConnectivityAlgo( adjacency_list );

    // Test
    fmt::print( "\n\nList of SCC\n{}\n", tarjan_scc.scc_list );

} // end of run
} // namespace Tarjan