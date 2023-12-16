require "json"
require 'pathname'

package = JSON.parse(File.read(File.join(__dir__, "package.json")))
folly_compiler_flags = '-DFOLLY_NO_CONFIG -DFOLLY_MOBILE=1 -DFOLLY_USE_LIBCPP=1 -Wno-comma -Wno-shorten-64-to-32'

Pod::Spec.new do |s|
  s.name         = "react-native-cpp"
  s.version      = package["version"]
  s.summary      = package["description"]
  s.homepage     = package["homepage"]
  s.license      = package["license"]
  s.authors      = package["author"]

  s.platforms    = { :ios => "11.0" }
  s.source       = { :git => "https://github.com/chrfalch/react-native-cpp.git", :tag => "#{s.version}" }

  s.source_files = ["ios/**/*.{h,m,mm}", "cpp/**/*.{hpp,cpp,c,h}"]

  # Resolve RN Project path
  app_path = ENV["APP_PATH"]
  if app_path
    package_json_path = File.join(app_path, 'package.json')
    if File.exist?(package_json_path)
      # Read and parse the package.json file
      package_json = JSON.parse(File.read(package_json_path))
      # Check if 'nativeDependencies' and 'cppSources' are present
      if package_json['nativeDependencies'] && package_json['nativeDependencies']['sources']
        # Create a new file in the library folder that should contain includes for the native files
        File.open(File.join(Dir.pwd, "ios", "react-native-cpp-includes.h"), "w") do |file|
          # Extract the C++ source file paths
          sources = package_json['nativeDependencies']['sources']
          # Check if sources is an array
          if sources.is_a?(Array)
            puts "Including/adding the following C++ sources to podspec:"
            puts sources
            # Assuming package_json_path is a string representing the root path
            library_pathname = Pathname.new(Dir.pwd)
            # Convert each path in sources to a relative path
            relative_sources = sources.map do |path|
              full_path = Pathname.new(File.join(app_path, path))
              full_path.relative_path_from(library_pathname).to_s              
            end          
            # Add the includes to the file
            relative_sources.each do |path|
              file.puts("#include \"#{File.basename(path)}\"")
            end
            # Include with the source files
            s.source_files = ["ios/**/*.{h,m,mm}", "cpp/**/*.{hpp,cpp,c,h}", relative_sources].flatten                      
          end
        end
      end
    end
  end

  # Use install_modules_dependencies helper to install the dependencies if React Native version >=0.71.0.
  # See https://github.com/facebook/react-native/blob/febf6b7f33fdb4904669f99d795eba4c0f95d7bf/scripts/cocoapods/new_architecture.rb#L79.
  if respond_to?(:install_modules_dependencies, true)
    install_modules_dependencies(s)
  else
  s.dependency "React-Core"
   
  # Don't install the dependencies when we run `pod install` in the old architecture.
  if ENV['RCT_NEW_ARCH_ENABLED'] == '1' then
    s.compiler_flags = folly_compiler_flags + " -DRCT_NEW_ARCH_ENABLED=1"
    s.pod_target_xcconfig    = {
        "HEADER_SEARCH_PATHS" => "\"$(PODS_ROOT)/boost\"",
        "OTHER_CPLUSPLUSFLAGS" => "-DFOLLY_NO_CONFIG -DFOLLY_MOBILE=1 -DFOLLY_USE_LIBCPP=1",
        "CLANG_CXX_LANGUAGE_STANDARD" => "c++17"
    }
    s.dependency "React-Codegen"
    s.dependency "RCT-Folly"
    s.dependency "RCTRequired"
    s.dependency "RCTTypeSafety"
    s.dependency "ReactCommon/turbomodule/core"
   end
  end    
end
