#pragma once
#include <string>
#include <vector>

namespace lve{
    class lvePipeline{
      public:
        lvePipeline(const std::string& vertFilepath, const std::string& fragFilepath);

      private:

        static std::vector<char> readFile(const std::string& filepath);

        void createGraphicPipeline(const std::string& vertFilepath, const std::string& fragFilepath);

    };
} //namespace lve