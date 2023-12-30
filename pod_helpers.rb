# pod_helpers.rb
require "json"
require 'pathname'

module RNCppPodHelpers
    # Returns a sub spec from the build config found in the path (configuration file)
    def self.extract_build_config(root, path, master_include_file, spec)
        # Read and parse the config.js file
        config = JSON.parse(File.read(path))    
        config_name = config["name"]
        if !config_name
            fail "RNCPP: The configuration file #{path} does not contain a name"
        end        
        
        # Check if 'sources' is present - we don't do anything if there are no sources
        if config['sources'] 
            sources = config['sources']
            relative_sources = []
            dependencies = []
            # Check if sources is an array
            if sources.is_a?(Array)
                # Create a new subspec 
                puts "Spec '#{config_name}': Auto-linking C++ from #{path}"

                spec.subspec config_name do |subspec|
                    # Assuming package_json_path is a string representing the root path
                    library_pathname = Pathname.new(Dir.pwd)
                    # Convert each path in sources to a relative path
                    relative_sources = sources.map do |path|
                        full_path = Pathname.new(File.join(root, path))
                        full_path.relative_path_from(library_pathname).to_s
                    end 

                    # Add to subspec source files
                    puts "Spec '#{config_name}': Subspec: Settings source files: #{relative_sources}"
                    subspec.source_files = ["example/cpp/*.h", "example/cpp/api/*"]
                    puts "Spec '#{config_name}': Subspec:  #{subspec.inspect}"                     

                    # Add the includes to the file
                    final_includes = []
                    relative_sources.each do |path|
                        # Can be a glob, let's de-glob it
                        include_files = Dir.glob(path)
                        include_files.each do |include_file|
                            if File.extname(include_file) == ".h" || File.extname(include_file) == ".hpp"
                                include_path = File.basename(include_file)
                                final_includes.push(include_path)
                                master_include_file.puts("#import \"#{include_path}\"")
                            end
                        end                  
                    end

                    puts "Spec '#{config_name}': Include: Adding include files: #{final_includes}"

                    # Get the dependencies
                    if config["ios"] && config["ios"]["dependencies"] && config["ios"]["dependencies"].is_a?(Array)
                        dependencies = config["ios"]["dependencies"]
                        dependencies.each do |dependency|
                            puts "Spec '#{config_name}': Adding dependency #{dependency}"
                            subspec.dependency dependency
                        end
                    end       

                    # Add additional configuration
                    if config["ios"] && config["ios"]["podspec"] && config["ios"]["podspec"].is_a?(Hash)
                        # For each key in the podspec, add it to the subspec
                        config["ios"]["podspec"].each do |key, value|
                            puts "Spec '#{config_name}': Adding podspec key #{key} with value #{value}"
                            subspec.send("#{key}=", value)
                        end
                    end
                    
                    # Return the new subspec
                    return subspec
                end
            else
                puts "RNCPP: Skipping #{path}, source config should be an array."
            end
        else
            puts "RNCPP: Skipping #{path}, no source files present in config."
        end
        return null
    end

    # This function will enumerate all react-native-cpp.json files in the path and return an array of all the sources.
    def self.enumerate_build_configs(path, spec)
        if !path
            return []
        end

        configs = Dir.glob(File.join(path, "/**/", "react-native-cpp.json"))
        if configs.length == 0
            puts "RNCPP: No react-native-cpp.json files found"
            return
        end

        # Log the number of configs found
        puts "RNCPP: Found #{configs.length} react-native-cpp.json files"

        subspecs = []
        
        # Ensure we have a react-native-cpp-includes.h file
        File.open(File.join(Dir.pwd, "ios", "react-native-cpp.h"), "w") do |include_file|
            # Enumerate all the config files
            configs.each do |configpath|
                subspec = RNCppPodHelpers.extract_build_config(path, configpath, include_file, spec)
                if subspec
                    subspecs.push(subspec)
                end
            end
        end

        return subspecs
    end
end