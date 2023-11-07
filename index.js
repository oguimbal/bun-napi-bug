
const testAddon = require('./build/Release/bufferbug.node')
console.log('-------------')
console.log('encoding', testAddon.getValueEncoding({valueEncoding: 'buffer'}))
module.exports = testAddon
