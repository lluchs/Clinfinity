# prepares everything for release

require 'fileutils'

include FileUtils

# remove unwanted stuff
Dir.glob '**/.no-release' do |f|
  rm_rf File.dirname(f)
end

# change definitions
Dir.glob 'Clinfinity.c4f/*/Scenario.txt' do |f|
  content = File.read f
  if content.gsub! /^(Definition\d=)Clinfinity\\/, '\1'
    File.open f, 'w' do |file|
      file.write content
    end
  end
end

