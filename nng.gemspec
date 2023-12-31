require File.expand_path("../lib/nng/version", __FILE__)

Gem::Specification.new do |s|
  s.name             = 'nng'
  s.version          = NNG::VERSION
  s.date             = '2021-11-27'
  s.authors          = ['Adib Saad']
  s.email            = ['adib.saad@gmail.com']
  s.licenses         = ['MIT']
  s.summary          = 'Ruby bindings for nng (nanomsg-ng).'
  s.homepage         = 'https://github.com/adibsaad/rbnng'
  s.files            = Dir['./ext/**/*.c'] +
                       Dir['./ext/**/*.h'] +
                       Dir['./lib/**/*.rb']
  s.extensions       = %w[ext/rbnng/extconf.rb]
  s.require_paths    = %w[ext lib]

  s.required_ruby_version = Gem::Requirement.new(">= 3.0.0")

  s.add_development_dependency "bundler"
  s.add_development_dependency "rake"
  s.add_development_dependency "rake-compiler"
end
